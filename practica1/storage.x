struct list {
	char file<256>;
  list *next;
}; typedef list *t_list;

struct listUsers{
	char user<256>;
	char ip<256>;
	char port<256>;
	listUsers *next;
}; typedef listUsers *t_listUsers;

program STORAGE {
	version STORAGEVER {
		int registerUser(string user<256>) = 1;
		int unregisterUser(string user<256>)  = 2;
		int publish(string user<256>, string file<256>, string desc<256>) = 3;
		int deleteContent(string user<256>, string file<256>) = 4;
		int connectUser(string user<256>, string ip<256>, string port<256>) = 5;
		int disconnectUser(string user<256>) = 6;
		int check_list_users(string user<256>) = 7;
		t_listUsers listConnectedUsers() = 8;
		int check_list_content(string user<256>, string userTarget<256>) = 9;
		t_list list_content(string userTarget<256>) = 10;
	} = 1;
} = 99;
