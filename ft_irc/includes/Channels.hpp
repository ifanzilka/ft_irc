#ifndef CHANNELS_HPP
# define CHANNELS_HPP

#include "IrcServer.hpp"

class Channels
{
    private:

        std::string                             _channelName;
        std::vector<ClientIrc *>                _clients;

    public:
        Channels(std::string name, std::string pass);
        ~Channels();



};


// class Channel2 {
//     private:
//         Postman*                                _postman;
//         std::string                             _channelName;
//         std::string                             _topic;
//         std::vector<User *>                     _userList;
//         std::vector<User *>                     _operList;
//         int                                     _modes;
//         int                                     _limit;

//     public:
//         Channel(std::string const &, Postman *);
//         ~Channel(){};

//         void                            addUser(User *user);
//         void                            addOper(User *user);
//         void                            removeOper(User *user);
//         void                            removeUserFromChannel(User *user);

//         void                            set_topic(const std::string &topic);
//         void                            set_limit(int);

//         std::string const               &get_topic() const;
//         std::string const               &get_channelname() const;
//         const std::vector<User *>       &get_userlist() const;
//         const std::vector<User *>       &get_operList() const;
//         int                             get_limit() const;

//         int                             get_count_of_users();
//         User*                           get_user_by_nickname(std::string nickname);
//         void                            sendAll(const std::string& msg, User*);

//         bool                            is_in_channel(User *user) const;
//         bool                            is_operator(User *user) const;

//         void                            set_mode(Mode);
//         void                            unset_mode(Mode);
//         bool                            has_mode(Mode) const;
//         std::string                     show_mode() const;
// };

#endif