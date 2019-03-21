#ifndef LOBBY_H
#define LOBBY_H

class lobby
{
     private :
          int foreign_key;

     public :
          
      //    lobby(string nickname, int user_id);
          
          string chat_history(string[] messages);
          int chat_time();
          void create_new_chat(string chat_id);
          bool delete_users(string user_id);
};
#endif
