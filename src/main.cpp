#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <cstring>
#include <dirent.h>

std::vector<std::string> getExeFiles(std::string &directory, bool &isRecursive) {
std::vector<std::string> exeFiles;
  
  DIR *dir;
  struct dirent *ent;

  dir = opendir(directory.c_str());
  if (dir == NULL) {
    std::cerr << "Dizin açılamadı: " << directory << std::endl;
    return exeFiles;
  }

  while ((ent = readdir(dir)) != NULL) {
    std::string fileName(ent->d_name);

    if (fileName != "." && fileName != "..") {
      size_t pos = fileName.find_last_of(".");
      if (pos != std::string::npos && fileName.substr(pos + 1) == "exe") {
        exeFiles.push_back(directory + "/" + fileName);
      }

      if (isRecursive && ent->d_type == DT_DIR) { // Alt dizin ise
        std::string subDir = directory + "/" + fileName;
        std::vector<std::string> subDirExeFiles = getExeFiles(subDir, isRecursive);
        exeFiles.insert(exeFiles.end(), subDirExeFiles.begin(), subDirExeFiles.end());
      }
    }
  }
  closedir(dir);
  return exeFiles;
}
std::string takeName(std::string dosyaYolu) {
  // Dosya yolunun sonuna kadar git
  size_t sonSlash = dosyaYolu.find_last_of('/');

  // Dosya adını kes
  if (sonSlash != std::string::npos) {
    return dosyaYolu.substr(sonSlash + 1, dosyaYolu.size() - sonSlash - 1);
  } else {
    // Dosya yolunda slash yoksa tümünü dosya adı olarak kabul et
    return dosyaYolu;
  }
}

std::string openfile(const std::string& file){

  std::ifstream dosya(file,std::ios::binary | std::ios::in);
  std::string content;
  content.assign(std::istreambuf_iterator<char>(dosya),
               std::istreambuf_iterator<char>()); 
  dosya.close();
  return content;
}

int main(int argc, char* argv[]) {
  const std::string user = std::getenv("USER");
  if (argc < 2) {
    std::cerr << " kullanım: ./main <dizin_yolu> [-r]" << std::endl;
    std::cerr << " -r seçeneği, alt dizinleri de taramasını sağlar." << std::endl;
    return 1;
  }

  std::string directory = argv[1];
  bool isRecursive = false;

  if (argc > 2 && strcmp(argv[2], "-r") == 0) {
    isRecursive = true;
  }

  std::vector<std::string> exeFiles = getExeFiles(directory, isRecursive);

  if (exeFiles.empty()) {
    std::cout << "Dizinde .exe dosyası bulunamadı." << std::endl;
  } else {
    std::cout << "Bulunan .exe dosyaları:" << std::endl;
    for (const std::string &exeFile : exeFiles) {
      std::cout << exeFile << std::endl;
    }
  }
  std::string dir = "/home/" + user + "/.zshrc";
  std::string dir2 = "/home/" + user + "/.bashrc";
  for (const std::string &exeFile : exeFiles) {
    std::string alias{"alias "+ takeName(exeFile)+"="+'"'+"wine "+exeFile+'"'};
    std::cout << alias << dir << std::endl;
    std::ofstream dosya(dir, std::ios::out | std::ios::app);
    dosya << alias << std::endl;
    dosya.close();
    std::ofstream dosya2(dir2, std::ios::out | std::ios::app);
    dosya2 << alias << std::endl;
    dosya2.close();
 
  }
  return 0;
}

