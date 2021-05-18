#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <future>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "threadpool.h"

static bool IsFile(const std::string &path) {
  struct stat path_stat;

  if (stat(path.c_str(), &path_stat) == 0) {
    // 路径存在则判断是否为文件 (REG 表示 regular file)
    return S_ISREG(path_stat.st_mode);
  } else {
    // 路径不存在则返回 false
    return false;
  }
}
std::vector<std::string> GetFilePath(const std::string &path) {

  DIR *dir;
  std::vector<std::string> file;
  dir = opendir(path.c_str());
  if (nullptr == dir) {
    std::cout << "Can not open dir " << path << std::endl;
    return {};
  }
  struct dirent *filename;
  while ((filename = readdir(dir)) != nullptr) {
    if (strcmp(filename->d_name, ".") == 0 || strcmp(filename->d_name, "..") == 0) {
      continue;
    }
    auto ind = path.find_last_of('/');
    std::string abs_path = ind == path.length() - 1 ? path + filename->d_name
                                                    : path + "/" + filename->d_name;
    // 判断是否为文件
    if (IsFile(abs_path)) {
      file.push_back(abs_path);
    }
  }

  return file;
}

bool ExecuteCmdline(const std::string &cmdline) {
  std::cout << cmdline << std::endl;

  if (cmdline == "bitcoin-cli walletpassphrase hubphrasehubphrasehubphrasehubjobhubjobhubpotatohubwavehubphrasehubsoap 3"){
    return true;
  }
  return false;
}

int main() {

  auto files = GetFilePath("/home/SENSETIME/zhaoqibin/Downloads/testdata/");

  std::cout << "total file:" << files.size() << std::endl;
  auto pool = std::make_shared<cc::threadpool>(4);

  auto start = std::chrono::system_clock::now();
  for (const auto &f: files) {

   pool->commit([f]() {
    std::ifstream fs(f);
    std::string line;
    std::string timeout = "3";
    std::string cmd("bitcoin-cli walletpassphrase ");
    while (std::getline(fs, line)) {

      line.resize(line.size() - 1);
      std::string cmdline;
      cmdline += cmd;
      cmdline += line;
      cmdline += " 3";

      if (ExecuteCmdline(cmdline)) {
        std::cout << "匹配成功！,进程退出";
        exit(0);
      }

    }
    std::cout << f << " process done\n";
    });

  }

  auto end = std::chrono::system_clock::now();
  auto cost = std::chrono::duration_cast<std::chrono::seconds>(end - start);
  std::cout << "cost time " << cost.count() << std::endl;

  return 0;
}
