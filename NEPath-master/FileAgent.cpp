#include "FileAgent.h"
#include <io.h>
#include <string>
#include <direct.h>

path FileAgent::read_csv(char const* filename) {
	if (!is_csv(filename)) {
		printf("%s is not csv file.", filename);
		return path();
	}
	ifstream inFile(filename, ios::in);
	if (!inFile) {
		printf("File %s does not exist.\n", filename);
		return path();
	}
	vector<double> x;
	vector<double> y;
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

void FileAgent::write_csv(const path& p, char const* filename) {
	if (!is_csv(filename)) {
		printf("%s is not csv file.", filename);
		return;
	}
	std::ofstream outFile(filename, std::ios::out);
	outFile << "x,y\n";
	for (int j = 0; j < p.length; ++j) {
		outFile << p.x[j] << ',' << p.y[j] << '\n';
	}
	outFile.close();
}

bool FileAgent::is_csv(char const* filename) {
	return filename[strlen(filename) - 1] == 'v'
		&& filename[strlen(filename) - 2] == 's'
		&& filename[strlen(filename) - 3] == 'c'
		&& filename[strlen(filename) - 4] == '.';
}

void FileAgent::write_csv(const paths& ps, char const* filename_pre, char const* filename_post/*=NULL*/) {
	if (!ps.size()) {
		return;
	}
	bool flag_new = !filename_post;
	if (flag_new) {
		filename_post = new char[5];
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
	if (flag_new) {
		delete[] filename_post;
		filename_post = NULL;
	}
}

vector<string> FileAgent::get_AllFiles(char const* path, bool folder/*=false*/, char const* filename_pre/*=NULL*/, char const* filename_post/*=NULL*/) {
	vector<string> files;
	string append_filename = "\\";
	if (filename_pre) {
		append_filename += filename_pre;
	}
	append_filename += "*";
	if (filename_post) {
		append_filename += filename_post;
	}

	//�ļ����
	long long hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append(append_filename).c_str(), &fileinfo)) != -1) {
		do {
			if (fileinfo.name[0] == '.') {
				continue;
			}
			if (folder && !(fileinfo.attrib & _A_SUBDIR)) {
				continue;
			}
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
	return files;
}

void FileAgent::delete_AllFiles(const char* path) {
	//��Ŀ¼�������"\\*.*"���е�һ������
	string dir(path);
	string newDir = dir + "\\*.*";
	//���ڲ��ҵľ��
	intptr_t handle;
	struct _finddata_t fileinfo;
	//��һ�β���
	handle = _findfirst(newDir.c_str(), &fileinfo);

	if (handle == -1) { // ���ļ���
		return;
	}

	do
	{
		if (fileinfo.attrib & _A_SUBDIR) {//���Ϊ�ļ��У������ļ���·�����ٴα���
			if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
				continue;

			// ��Ŀ¼�������"\\"����������Ŀ¼��������һ������
			newDir = dir + "\\" + fileinfo.name;
			delete_AllFiles(newDir.c_str()); // �ȱ���ɾ���ļ����µ��ļ�����ɾ���յ��ļ���
			_rmdir(newDir.c_str());
			/*
			cout << newDir.c_str() << endl;
			if (_rmdir(newDir.c_str()) == 0) {//ɾ�����ļ���
				cout << "delete empty dir success" << endl;
			}
			else {
				cout << "delete empty dir error" << endl;
			}*/
		}
		else {
			string file_path = dir + "\\" + fileinfo.name;
			remove(file_path.c_str());
			/*
			cout << file_path.c_str() << endl;
			if (remove(file_path.c_str()) == 0) {//ɾ���ļ�
				cout << "delete file success" << endl;
			}
			else {
				cout << "delete file error" << endl;
			}*/
		}
	} while (!_findnext(handle, &fileinfo));

	_findclose(handle);
	return;
}

void FileAgent::mkdir(const char* path, bool clear/*=false*/) {
	if (clear) {
		delete_AllFiles(path);
	}
	_mkdir(path);
}