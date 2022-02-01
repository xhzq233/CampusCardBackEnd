//
// Created by 夏侯臻 on 2022/1/14.
//

#ifndef CAMPUSCARDBACKEND_FILEMANAGER_H
#define CAMPUSCARDBACKEND_FILEMANAGER_H

/*
 * Singleton FileManager
 * */
class FileManager {

private:
    //make these constructors not accessible
    FileManager()= default;
    ~FileManager() = default;
public:
    //delete these copy methods
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
    /*
     * get the only instance of [FileManager]
     * In **C++11**, it is thread safe. According to the standard, §6.7 [stmt.dcl] p4:
     * If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.
     * */
    static FileManager& getInstance();
};


#endif //CAMPUSCARDBACKEND_FILEMANAGER_H
