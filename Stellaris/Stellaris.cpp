// Stellaris.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <steam_api.h>
#include <rapidjson.h>
#include <document.h>
#include <reader.h>
#include <stringbuffer.h>
#include <writer.h>
#include <encodings.h>
#include <pointer.h>
#include <filereadstream.h>
#include <Windows.h>
#include <shlobj.h>
#include <direct.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <list>
#include <algorithm>
#include "xxhash.h"
#include "bkutf8.h"
#include "sqlite3.h"
#include <io.h>
#include <fcntl.h>

using namespace rapidjson;
using namespace std;

//class DownloadCallback
//{
//public:
//    STEAM_CALLBACK(DownloadCallback, onDownloadOver, DownloadItemResult_t);
//};
//
//void DownloadCallback::onDownloadOver(DownloadItemResult_t *p)
//{
//
//}

XXH64_hash_t calcul_hash_streaming(wstring fh)
{
    /* create a hash state */
    XXH64_state_t* const state = XXH64_createState();
    if (state == NULL)
        abort();

    FILE* fp = _wfopen(fh.c_str(), L"rb");
    fseek(fp, 0, SEEK_END);

    size_t const bufferSize = ftell(fp);

    if (bufferSize == 0)
    {
        XXH64_freeState(state);
        fclose(fp);
        return 0;
    }

    void* const buffer = malloc(bufferSize);
    if (buffer == NULL) abort();

    /* Initialize state with selected seed */
    XXH64_hash_t const seed = 0;   /* or any other value */
    if (XXH64_reset(state, seed) == XXH_ERROR) abort();

    fseek(fp, 0, SEEK_SET);
    fread(buffer, bufferSize, 1, fp);
    fclose(fp);

    if (XXH64_update(state, buffer, bufferSize) == XXH_ERROR) abort();

    /* Produce the final hash value */
    XXH64_hash_t const hashvalue = XXH64_digest(state);

    /* State could be re-used; but in this example, it is simply freed  */
    free(buffer);
    XXH64_freeState(state);

    return hashvalue;
}

void getFiles(const wstring& parentDir, list<wstring> &files)
{
    WIN32_FIND_DATAW data;
    HANDLE h;
    h = FindFirstFileExW((parentDir + L"*").c_str(), FindExInfoBasic, &data, FindExSearchNameMatch, NULL, FIND_FIRST_EX_LARGE_FETCH);
    if (h == INVALID_HANDLE_VALUE)
        return;
    while (FindNextFileW(h, &data))
    {
        if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            files.push_back(parentDir + data.cFileName);
        }
        else if (wcscmp(data.cFileName, L".") && wcscmp(data.cFileName, L".."))
        {
            wstring p = parentDir + data.cFileName + L"/";
            getFiles(p, files);
        }
    }
    FindClose(h);
}

#pragma comment(lib, "shell32.lib")

wstring GetMyDocumentPath()
{
    WCHAR my_documents[MAX_PATH];
    HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
    if (result != S_OK)
    {
        cout << L"Error Get MyDocument Path" << endl;
        return L"";
    }
    else
    {
        return my_documents;
    }
}

struct ModIdInfo
{
    wstring id;
    wstring position;
};

struct ModInfo
{
    wstring id;
    PublishedFileId_t steamId;
    wstring displayName;
    wstring dir;
};

int main()
{
    constexpr char cp_utf8[] = ".65001";
    setlocale(LC_ALL, cp_utf8);
    _setmode(_fileno(stdout), _O_WTEXT);
    cout.sync_with_stdio();
    wstring doc_path = GetMyDocumentPath();
    wstring sql_path = doc_path + L"\\Paradox Interactive\\Stellaris\\launcher-v2.sqlite";
    FILE* fsql = _wfopen(sql_path.c_str(), L"rb");
    if (!fsql)
    {
        cout << "launcher-v2.sqlite is not found in default path (or be locked), please input the fullpath of launcher-v2.sqlite:";
        wcin >> sql_path;
        fsql = _wfopen(sql_path.c_str(), L"rb");
        while (fsql == NULL)
        {
            cout << "launcher-v2.sqlite is not found (or be locked), please input the fullpath of launcher-v2.sqlite:";
            wcin >> sql_path;
            fsql = _wfopen(sql_path.c_str(), L"rb");
        }
    }
    fclose(fsql);
    sqlite3* sql = nullptr;
    int sr = sqlite3_open16(sql_path.c_str(), &sql);
    if (sr != SQLITE_OK)
    {
        cout << "Open sqlite database failed, the file maybe locked, try to close Stellaris Launcher." << endl;
        if (!sql)
            sqlite3_close(sql);
        return -1;
    }
    sqlite3_stmt* st_curplayset;
    sr = sqlite3_prepare16_v2(sql, L"SELECT id,name from \"playsets\" WHERE isActive=1", -1, &st_curplayset, NULL);
    sr = sqlite3_step(st_curplayset);
    if (sr != SQLITE_DONE && sr!= SQLITE_ROW)
    {
        cout << "internal error" << endl;
        sqlite3_close(sql);
        return -1;
    }
    wstring id, name;
    while (sr != SQLITE_DONE)
    {
        id = (wchar_t*)sqlite3_column_text16(st_curplayset, 0);
        name = (wchar_t*)sqlite3_column_text16(st_curplayset, 1);
        sr = sqlite3_step(st_curplayset);
    }
    sqlite3_finalize(st_curplayset);
    wcout << L"Current playset:" << name << endl;
    sqlite3_stmt* st_mods;
    wstring query = L"SELECT modId,position from \"playsets_mods\" WHERE playsetId=\"" + id + L"\"";
    sr = sqlite3_prepare16_v2(sql, query.c_str(), -1, &st_mods, NULL);
    sr = sqlite3_step(st_mods);
    if (sr != SQLITE_DONE && sr != SQLITE_ROW)
    {
        cout << "internal error" << endl;
        sqlite3_close(sql);
        return -1;
    }
    vector<ModIdInfo> modIdInfo;
    while (sr != SQLITE_DONE)
    {
        wstring modId, modPosition;
        modId = (wchar_t*)sqlite3_column_text16(st_mods, 0);
        modPosition = (wchar_t*)sqlite3_column_text16(st_mods, 1);
        modIdInfo.push_back(ModIdInfo{ modId, modPosition });
        sr = sqlite3_step(st_mods);
    }
    sqlite3_finalize(st_mods);
    sort(modIdInfo.begin(), modIdInfo.end(), [](const ModIdInfo& a, const ModIdInfo& b) {return a.position < b.position; });
    sqlite3_stmt* st_modInfo;
    sr = sqlite3_prepare16_v2(sql, L"SELECT id,steamId,displayName,dirPath from \"mods\"", -1, &st_modInfo, NULL);
    sr = sqlite3_step(st_modInfo);
    if (sr != SQLITE_DONE && sr != SQLITE_ROW)
    {
        cout << "internal error" << endl;
        sqlite3_close(sql);
        return -1;
    }
    unordered_map<wstring, ModInfo> modInfo;
    while (sr != SQLITE_DONE)
    {
        wstring modId, modSteamId, modName, modDir;
        modId = (wchar_t*)sqlite3_column_text16(st_modInfo, 0);
        wchar_t *steamId = (wchar_t*)sqlite3_column_text16(st_modInfo, 1);
        modName = (wchar_t*)sqlite3_column_text16(st_modInfo, 2);
        modDir = (wchar_t*)sqlite3_column_text16(st_modInfo, 3);
        PublishedFileId_t iid = 0;
        if (steamId != nullptr)
        {
            wstringstream sm;
            modSteamId = steamId;
            sm << modSteamId;
            sm >> iid;
        }
        modInfo[modId] = ModInfo{ modId , iid , modName , modDir };
        sr = sqlite3_step(st_modInfo);
    }
    sqlite3_finalize(st_modInfo);
    sqlite3_close(sql);

    wcout << "Mods in current playset:" << endl;
    for (int i = 0; i < modIdInfo.size(); i++)
    {
        auto& m = modIdInfo[i];
        wcout << L"\t[" << i << L"]" << modInfo[m.id].displayName;
        if (modInfo[m.id].steamId == 0)
        {
            wcout << " (Non-steam mod)";
        }
        wcout << endl;
    }

    bool r = SteamAPI_Init();
    if (!r)
    {
        wcout << "SteamAPI init failed" << endl;
        return -1;
    }
    
    FILE* fp2 = fopen("hash.json", "w");
    fprintf(fp2, "{\n");
    wcout << "Starting check mods..." << endl;
    auto ugc = SteamUGC();
    bool firstwrite = true;
    //mkdir("mod_merge");
    for (int i = 0; i < modIdInfo.size(); i++)
    {
        ModInfo info = modInfo[modIdInfo[i].id];
        bool hasDownload = false;
        PublishedFileId_t mod = info.steamId;
        if (mod == 0)
        {
            wcout << L"Skip Non-Steam mod: " << info.displayName << endl;
            continue;
        }
        wstring prefix = L"[" + std::to_wstring(i) + L"]" + info.displayName + L"\t";
        wcout << prefix;
        auto state = ugc->GetItemState(mod);
        if (state == k_EItemStateNone)
        {
            //need to subscribe
            auto call = ugc->SubscribeItem(mod);
            Sleep(100);
            bool dr = ugc->DownloadItem(mod, true);
            if (!dr)
            {
                wcout << "Download failed" << endl;
                continue;
            }
        }
        else if (!(state & k_EItemStateInstalled) || ((state & k_EItemStateInstalled) && (state & k_EItemStateNeedsUpdate)))
        {
            bool dr = ugc->DownloadItem(mod, true);
            if (!dr)
            {
                wcout << "Download failed" << endl;
                continue;
            }
        }
        else if (state & k_EItemStateDownloadPending)
        {
            bool dr = ugc->DownloadItem(mod, true);
            if (!dr)
            {
                wcout << "Download failed" << endl;
                continue;
            }
        }
        else if (state & k_EItemStateDownloading)
        {
        }
        else
        {
            //cout << "Updated" << endl;
            //hasDownload = true;
            //string cmd = "xcopy \"" + mod_dir + to_string(mod) + "\" mod_merge /E /Y /Q";
            //system(cmd.c_str());
            //continue;
            
            //we still try to update it
            ugc->DownloadItem(mod, true);
        }
        if (!hasDownload)
        {
            uint64 downloaded, total;
            do
            {
                wprintf(L"\r");
                wcout << prefix;
                Sleep(1000);
                ugc->GetItemDownloadInfo(mod, &downloaded, &total);
                if(total > 0)
                    wprintf(L"%I64u/%I64u  %.2f%%\t\t", downloaded, total, 100.0f * (float)downloaded / (float)total);
            } while (downloaded < total);
            wprintf(L"\r");
            wcout << prefix;
            wcout << "Download over\t\t\t" << endl;
            Sleep(100);
        }
        //string cmd = "xcopy \"" + mod_dir + to_string(mod) + "\" mod_merge /E /Y /Q";
        //system(cmd.c_str());
        list<wstring> files;
        //info.dir may be wrong, should get it from steam api
        //char dir[MAX_PATH];
        //uint64 modsize;
        //uint32 timestamp;
        //ugc->GetItemInstallInfo(mod, &modsize, dir, MAX_PATH, &timestamp);
        //getFiles(UniFromUTF8(dir) + L"/" + to_wstring(mod) + L"/", files);
        getFiles(info.dir + L"/", files);
        XXH64_hash_t final_hash = 0;
        for (auto& f : files)
        {
            auto hashvalue = calcul_hash_streaming(f);
            final_hash ^= hashvalue;
            //string ff =UniToUTF8(f).substr(prelen);
            //string hh = to_string(hashvalue);
            //fprintf(fp2, "\t\"\s\": \s,\n", ff, hh);
            //SetValueByPointer(d2, ff.c_str(), to_string(hashvalue).c_str());
        }
        string u8name = UniToUTF8(info.displayName);
        if (!firstwrite)
        {
            fprintf(fp2, ",\n");
        }
        fprintf(fp2, "\t\"%I64u\":{\n\t\t\"name\": \"%s\",\n\t\t\"hash\":%I64u\n\t}", mod, u8name.c_str(), final_hash);
        firstwrite = false;
    }
    fprintf(fp2, "\n}");
    fclose(fp2);

    wcout << endl;
    system("pause");
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
