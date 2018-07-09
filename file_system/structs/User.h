//
// Created by capitalg on 2018/7/9.
//

#ifndef FILE_SYSTEM_USER_H
#define FILE_SYSTEM_USER_H
class User {
public:
    int uid;
    std::string uname;
    std::string passwd;

    explicit User(int uid, const std::string &uname, const std::string &passwd) : uid(uid), uname(uname), passwd(passwd) {}
};
#endif //FILE_SYSTEM_USER_H
