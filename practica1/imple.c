#include "imple.h"

//DATABASE FILE
#define DATABASE_NAME "database.db"
char db[MAX_LINE] = DATABASE_NAME;

void serverMsg(char * msg){
  printf("%s\n", msg);
  printf("s> ");
  fflush( stdout );
}

int main(int argc, char const *argv[]) {
  printf("%d\n", unregisterUser("user2"));

  return(0);
}


//OPERACIONES

// ------- REGISTER -------
int registerUser(char * user){
  if(!validUsername(user)){//comprueba si no contiene caracteres no válidos para el nombre de usuario
    return 2;
  }
  char userFormat[MAX_FILE_LINE];
  sprintf(userFormat, ":::%s\n", user); //los usuarios empiezan con este formato ":::username"

  FILE* fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
    serverMsg("A new database will be created");
    fd = fopen(db, "w");
    if (fd == NULL){
      perror("Couldn't create a new database");
      return 2;
    }
    if (fprintf(fd, "%s", userFormat) == -1){ //se le incluye al final
      perror("fprintf");
      fclose(fd);
      return 2; //error
    }

    fclose(fd);
    return 0; //todo ok
  }
  //existía la base de datos

  //buscar a usuario,linea por linea
  int userLine = searchUserPos(fd, userFormat);
  if (userLine == -1){ //no estaba en la lista
    if (fprintf(fd, "%s", userFormat) == -1){ //se le incluye al final
      perror("fprintf");
      fclose(fd);
      return 2; //error
    }
    fclose(fd);
    return 0; //todo ok
  }
  //ya estaba incluido
  fclose(fd);
  return 1;
}


// ------- UNREGISTER -------
int unregisterUser(char * user){
  if(!validUsername(user)){ //comprueba si no contiene caracteres no válidos para el nombre de usuario
    return 2;
  }
  char userFormat[MAX_FILE_LINE];
  sprintf(userFormat, ":::%s\n", user); //los usuarios empiezan con este formato ":::username"

  printf("%s\n", userFormat );

  FILE* fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
      perror("Not database existing");
      return 2;
  }
  //existía la base de datos

  //buscar a usuario,linea por linea
  int userLine = searchUserPos(fd, userFormat);
  printf("userLine %d\n", userLine);
  if (userLine == -1){ //no estaba en la lista
    fclose(fd);
    return 2; //error
  }
  //borrar al usuario y sus ficheros
  return deleteUser(fd, userLine);
  // fclose(fd);
  // return 1;
}

//Buscar línea del user
int searchUserPos(FILE* fd, char *userFormat){
  char str[MAX_FILE_LINE];
  int line = 0;
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if (strcmp(str, userFormat) == 0)
      return line;
  }
  return -1;
}
//busca la línea en la que está el siguiente al pasado por parámetro
int searchNextUserPos(FILE* fd, int userLine){ //devuelve -1 si no hay más usuarios, y en caso contrario la posición del siguiente
  char str[MAX_FILE_LINE];
  int line = -1;
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if (str[0] == ':' && str[1] == ':' && str[2] == ':'){
      return line + userLine;
    }
  }
  return -1;
}

//función para borrar a un usuario y sus canciones
int deleteUser(FILE* fd, int userLine){
  int nextUserLine = searchNextUserPos(fd, userLine);
  printf("nextUserLine %d\n", nextUserLine);
  int ctr = 0;
  FILE *fd2;
  char str[MAX_FILE_LINE], temp[] = "temp.txt";
  fd2 = fopen(temp, "w"); // open the temporary file in write
  if (fd2 == NULL){
    perror("fopen, fd2");
    fclose(fd);
    return 2;
  }
  fseek(fd, 0, SEEK_SET);
  if (nextUserLine == -1){
    //borrar todo después de userLine
    while (!feof(fd)) {
      strcpy(str, "\0");
      fgets(str, MAX_FILE_LINE, fd);
      if (!feof(fd))
      {
        ctr++;
         if (ctr < userLine){
             fprintf(fd2, "%s", str);
         }
      }
    }


  }else{
    //borrar entre userLine y nextUserLine
    while (!feof(fd)) {
      strcpy(str, "\0");
      fgets(str, MAX_FILE_LINE, fd);
      if (!feof(fd)){
        ctr++;
        if (ctr < userLine || ctr > nextUserLine){
           fprintf(fd2, "%s", str);
        }
      }
    }

  }
  fclose(fd2);
  fclose(fd);
  remove(DATABASE_NAME);  		// remove the original file
  rename(temp, DATABASE_NAME); 	// rename the temporary file to original name
  return 0;
}

//formato de nombre no permitido
int validUsername(char *name){
  if (strlen(name)>=3){
    if (name[0] == ':' && name[1] == ':' && name[2] == ':')
      return 0;
  }
  return 1; //está permitido
}

// ZONA DE MENSAJES EN LA PANTALLA
void print_usage() {
	    printf("Usage: server -p puerto \n");
}

void printInitServer(struct sockaddr_in server_addr){

	char host[256];
	char *IP;
	struct hostent *host_entry;
	gethostname(host, sizeof(host)); //find the host name
	host_entry = gethostbyname(host); //find host information
	IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
	printf("s> ");
	printf("init server %s:%d\n", IP, ntohs(server_addr.sin_port));
	printf("s> ");
	fflush(stdout);
	//Fin init server
}
