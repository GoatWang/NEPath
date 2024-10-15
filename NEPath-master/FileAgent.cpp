#include "FileAgent.h"
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <cstring>

#ifdef _WIN32
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <dirent.h>
#endif

using namespace std;

// Read a path from a .csv file
// The .csv file has the following form:
// x,y,
// data_x1,data_y1,
// data_x2,data_y2,
// ...
path FileAgent::read_csv(const char* filename) {
    if (!is_csv(filename)) {
        printf("%s is not a csv file.\n", filename);
        return path();
    }

    ifstream inFile(filename, ios::in);
    if (!inFile) {
        printf("File %s does not exist.\n", filename);
        return path();
    }

    vector<double> x, y;
    while (!inFile.eof()) {
        bool end = false;
        double t = readcsv_double(inFile, end);
        if (!end) {
            x.push_back(t);
            y.push_back(readcsv_double(inFile));
        }
    }
    return path(x.data(), y.data(), x.size());
}

// Write a path into a .csv file
void FileAgent::write_csv(const path& p, const char* filename) {
    if (!is_csv(filename)) {
        printf("%s is not a csv file.\n", filename);
        return;
    }

    ofstream outFile(filename, ios::out);
    outFile << "x,y\n";
    for (int j = 0; j < p.length; ++j) {
        outFile << p.x[j] << ',' << p.y[j] << '\n';
    }
    outFile.close();
}

// Determine whether a file is a .csv file
bool FileAgent::is_csv(const char* filename) {
    return filename[strlen(filename) - 1] == 'v'
        && filename[strlen(filename) - 2] == 's'
        && filename[strlen(filename) - 3] == 'c'
        && filename[strlen(filename) - 4] == '.';
}

// Write paths into .csv files with a common prefix and suffix for filenames
void FileAgent::write_csv(const paths& ps, const char* filename_pre, const char* filename_post /*=NULL*/) {
    if (!ps.size()) return;

    bool flag_new = !filename_post;
    if (flag_new) {
        filename_post = ".csv";
    }

    string filename(filename_pre);
    int num = 0;
    int n = ps.size();
    while (n) {
        ++num;
        n /= 10;
    }
    for (int i = 0; i < num; ++i) {
        filename += "0";
    }
    filename += filename_post;

    for (int i = 0; i < ps.size(); ++i) {
        int j = i;
        for (int k = 0; k < num; ++k) {
            filename[filename.size() - strlen(filename_post) - k - 1] = '0' + (j % 10);
            j /= 10;
        }
        write_csv(ps[i], filename.data());
    }
}

// Get all filenames in a folder
vector<string> FileAgent::get_AllFiles(const char* path, bool folder /*=false*/, const char* filename_pre /*=NULL*/, const char* filename_post /*=NULL*/) {
    vector<string> files;
    string append_filename = "/";

    if (filename_pre) append_filename += filename_pre;
    append_filename += "*";
    if (filename_post) append_filename += filename_post;

#ifdef _WIN32
    long long hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append(append_filename).c_str(), &fileinfo)) != -1) {
        do {
            if (fileinfo.name[0] == '.') continue;
            if (folder && !(fileinfo.attrib & _A_SUBDIR)) continue;
            files.push_back(p.assign(path).append("/").append(fileinfo.name));
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
#else
    DIR* dir;
    struct dirent* entry;
    struct stat info;

    if ((dir = opendir(path)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            string fileName = entry->d_name;
            if (fileName == "." || fileName == "..") continue;

            string fullPath = string(path) + "/" + fileName;
            stat(fullPath.c_str(), &info);

            if (folder && !S_ISDIR(info.st_mode)) continue;
            files.push_back(fullPath);
        }
        closedir(dir);
    }
#endif

    return files;
}

// Delete all files in a folder
void FileAgent::delete_AllFiles(const char* path) {
#ifdef _WIN32
    string dir(path);
    string newDir = dir + "\\*.*";
    intptr_t handle;
    struct _finddata_t fileinfo;
    handle = _findfirst(newDir.c_str(), &fileinfo);

    if (handle == -1) return;

    do {
        if (fileinfo.attrib & _A_SUBDIR) {
            if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0) continue;

            newDir = dir + "\\" + fileinfo.name;
            delete_AllFiles(newDir.c_str());
            _rmdir(newDir.c_str());
        } else {
            string file_path = dir + "\\" + fileinfo.name;
            remove(file_path.c_str());
        }
    } while (!_findnext(handle, &fileinfo));

    _findclose(handle);
#else
    DIR* dir;
    struct dirent* entry;
    struct stat info;

    if ((dir = opendir(path)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            string fileName = entry->d_name;
            if (fileName == "." || fileName == "..") continue;

            string fullPath = string(path) + "/" + fileName;
            stat(fullPath.c_str(), &info);

            if (S_ISDIR(info.st_mode)) {
                delete_AllFiles(fullPath.c_str());
                rmdir(fullPath.c_str());
            } else {
                remove(fullPath.c_str());
            }
        }
        closedir(dir);
    }
#endif
}

// Create a directory
void FileAgent::mkdir(const char* path, bool clear /*=false*/) {
    if (clear) {
        delete_AllFiles(path);
    }
#ifdef _WIN32
    _mkdir(path);
#else
    ::mkdir(path, 0777); // UNIX permissions set to 0777
#endif
}
