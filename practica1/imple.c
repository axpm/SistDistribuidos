#include "imple.h"

//DATABASE FILE
char db[MAX_LINE] = DATABASE_NAME;

int main(int argc, char const *argv[]) {
  bool noMore = false;
  int n = 6;
  char userTarget[MAX_LINE] = "user4";
	int firstLine = 0, lastLine = 0;
  char file[MAX_LINE];

  findContentUser(userTarget, &firstLine, &lastLine);

  printf("firstLine: %d; lastLine: %d\n", firstLine, lastLine);


  for (int i = 0; i < n; i++){
    fillContentUser(file, &firstLine, lastLine, &noMore);
    printf("round: %d\n", i+1 );
    if (noMore){
      sprintf(file, " ");
      printf("file: %s\n", file );
      printf("\n" );
    }else{
      printf("file: %s\n", file );
      printf("\n" );
    }
    firstLine++;
  }

  return(0);
}


//OPERACIONES

// ------- REGISTER -------
int registerUser(char * user){
  if(!validUsername(user)){//comprueba si no contiene caracteres no válidos para el nombre de usuario
    return 2;
  }
  char userFormat[MAX_LINE];
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
      // perror("fprintf");
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
  char userFormat[MAX_LINE];
  sprintf(userFormat, ":::%s\n", user); //los usuarios empiezan con este formato ":::username"

  FILE* fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
      perror("Not database existing");
      return 2;
  }
  //existía la base de datos

  //buscar a usuario,linea por linea
  int userLine = searchUserPos(fd, userFormat);
  if (userLine == -1){ //no estaba en la lista
    fclose(fd);
    return 2; //error
  }
  //borrar al usuario y sus ficheros
  return deleteUser(fd, userLine);
}

// ------- PUBLISH content-------
int publish(char *user, char *file, char *desc){
  if (strcmp(file, "") == 0)//si el nombre del fichero es vacío
    return 4;
  FILE* fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
    perror("Not database existing");
    return 4;
  }
  char userFormat[MAX_LINE];
  sprintf(userFormat, ":::%s\n", user); //los usuarios empiezan con este formato ":::username"
  int userLine = isConnected(fd, userFormat); //mira si está conectado un usuario y si lo está devuelve la línea en la que está, si no existe -1 //searchUserPos(fd, userFormat);
  if(userLine == -1){ //el usuario no existe
    fclose(fd);
    return 1;
  }else if(userLine == 0 ){ //el usuario no está connectado
    fclose(fd);
    // printf("%s\n", "No conectado");
    return 2;
  }
  //else está conectado y en la línea userLine
  int nextUserLine = searchNextUserPos(fd, userLine); //saber el siguiente usuario
  char fileFormat[MAX_FILE_LINE];
  sprintf(fileFormat, "->%s||%s\n", file, desc); //juntar fichero y descripción en un string

  if (searchFile(fd, file, userLine, nextUserLine) > 0){ // si ya estaba el fichero
    fclose(fd);
    return 3;
  }
  //else no estaba y se incluye
  return addFile(fd, fileFormat, userLine, nextUserLine); //añade el archivo al final del usuario, devuelve 4 si error y 0 si todo ok

  // return 0;
} //end of publish

// ------- DELETE content -------
int deleteContent(char *user, char *file){
  if (strcmp(file, "") == 0)//si el nombre del fichero es vacío
    return 4;
  FILE* fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
    perror("Not database existing");
    return 4;
  }
  char userFormat[MAX_LINE];
  sprintf(userFormat, ":::%s\n", user); //los usuarios empiezan con este formato ":::username"
  int userLine = isConnected(fd, userFormat); //mira si está conectado un usuario y si lo está devuelve la línea en la que está, si no existe -1 //searchUserPos(fd, userFormat);

  if(userLine == -1){ //el usuario no existe
    fclose(fd);
    return 1;
  }else if(userLine == 0 ){ //el usuario no está connectado
    fclose(fd);
    return 2;
  }
  //else está conectado y en la línea userLine
  int nextUserLine = searchNextUserPos(fd, userLine); //saber el siguiente usuario
  int fileLine = searchFile(fd, file, userLine, nextUserLine);

  if ( fileLine == 0){ // si no estaba el fichero devuelve 0, en caso contrario devuelve la línea
    fclose(fd);
    return 3;
  }
  //else sí estaba y se elimina
  return deleteFile(fd, fileLine); //devuelve 4 si error y 0 si todo ok
  // return 0;
}

// ------- LIST_USERS -------
int list_users(char *user){
  FILE* fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
    perror("Not database existing");
    return 3;
  }
  char userFormat[MAX_LINE];
  sprintf(userFormat, ":::%s\n", user); //los usuarios empiezan con este formato ":::username"
  int userLine = isConnected(fd, userFormat); //mira si está conectado un usuario y si lo está devuelve la línea en la que está, si no existe -1 //searchUserPos(fd, userFormat);
  if(userLine == -1){ //el usuario no existe
    fclose(fd);
    return 1;
  }else if(userLine == 0 ){ //el usuario no está connectado
    fclose(fd);
    return 2;
  }

  return 0; //todo OK
}
// ------- LIST_USERS 2 (rellena la info) -------
void fillUserInfo(char *user, char * ip, char *port, int *userLine, int *nextUserLine, bool *noMore){
  if(*userLine == 0){
    *userLine = 1;
  }else{
    *userLine = *nextUserLine;
  }
  FILE *fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
    perror("Not database existing");
    *noMore = true;
    return;
  }
  int line = 0;
  if (*userLine == -1){//no hay más usuarios
    *noMore = true;
    return;
  }
  *nextUserLine = searchNextUserPos(fd, *userLine);
  if(*userLine == 1 && *nextUserLine == -1){ //no hay usuarios
    *noMore = true;
    return;
  }
  char str[MAX_FILE_LINE];
  fseek(fd, 0, SEEK_SET);
  bool copied = false;
  //buscamos en el fichero
  while(fgets(str, sizeof(str), fd) && !copied) {
    line ++;
    if (line == *userLine ){ //si está en la línea del usuario, se comprueban sus datos
      char copy[MAX_FILE_LINE];
      //guardar el user
      strcpy(copy, str);
      char *ptr = strtok(copy, ":::");
      ptr = strtok(ptr, "\n"); //borrar el \n final
      sprintf(user, "%s", ptr);
      if( isConnectedWithoutFD(fd, str) <= 0 ) {
       *userLine = searchNextUserPosWithoutFD(fd, *userLine);
      }else{
        *nextUserLine = searchNextUserPosWithoutFD(fd, *userLine);
      }
    }//fin if
    if(line == *userLine + 1){
      if(str[0] == '$'){
        copied = true;
        char copy[MAX_FILE_LINE];
        strcpy(copy,str);
        //cogemos el primer caracter
        char *ptr = strtok(copy, "$");
        //cogemos la cadena que continua el simbolo
        strcpy(copy,ptr);
        //cogemos el puerto
        ptr = strtok(copy, ":");
        strcpy(ip, ptr);
        ptr = strtok(NULL, ":");
        ptr = strtok(ptr, "\n"); //borrar el \n final
        sprintf(port, "%s", ptr);
      }
      // else{ //no es la info de IP:port
      //   sprintf(user, " ");
      //   sprintf(ip, " ");
      //   sprintf(port, " ");
      // }
    }

  } //fin while
  //fin y cerramos el fichero
  fclose(fd);
  return ;
} //fin de LIST_USERS (2)

// ------- LIST_CONTENT -------
int list_content(char *user, char *userTarget){
  FILE* fd = fopen(db, "r+"); //abrir para lectura y escritura
  if (fd == NULL){ //NO existía la base de datos
    perror("Not database existing");
    return 3;
  }
  char userFormat[MAX_LINE];
  sprintf(userFormat, ":::%s\n", user); //los usuarios empiezan con este formato ":::username"
  int userLine = isConnectedWithoutFD(fd, userFormat); //mira si está conectado un usuario y si lo está devuelve la línea en la que está, si no existe -1 //searchUserPos(fd, userFormat);
  if(userLine == -1){ //el usuario no existe
    fclose(fd);
    return 1;
  }else if(userLine == 0 ){ //el usuario no está connectado
    fclose(fd);
    return 2;
  }

  sprintf(userFormat, ":::%s\n", userTarget); //los usuarios empiezan con este formato ":::username"
  userLine = searchUserPos(fd, userFormat);
  if(userLine == -1){ //el usuario del que se quiere saber la no no existe
    fclose(fd);
    return 1;
  }

  fclose(fd);
  return 0; //todo OK
}//end list_content

// ------- LIST_CONTENT (2)-------
void fillContentUser(char *file, int *firstLine, int lastLine, bool *noMore){
  FILE *fd = fopen(db, "r+"); //abrir para lectura y escritura);
  char str[MAX_FILE_LINE];
  int line = 0;
  if(*firstLine == 0){
    *noMore = true; //si no hay nada, estará vacío todo
  }
  if (lastLine == -1) {
    while(fgets(str, sizeof(str), fd) && *noMore == false) {
      line ++;
      if (line == *firstLine) {
        char *copy = strtok(str, "->");
        copy = strtok(copy, "||");
        strcpy(file, copy);
      }
    }
    if(*firstLine > line){
      *noMore = true;
    }
  }else{
    if(*firstLine == lastLine){
      *noMore = true;
    }
    while(fgets(str, sizeof(str), fd) && *noMore == false) {
      line ++;
      if (line == *firstLine && *noMore == false) {
        char *copy = strtok(str, "->");
        copy = strtok(copy, "||");
        strcpy(file, copy);
      }
    }
  }
  fclose(fd);
}//end list_content(2) (fillContentUser)


// ------- Funciones de apoyo -------
//Rellenar límites de contenido usuario
void findContentUser(char *user, int *firstLine, int* lastLine){
  FILE *fd = fopen(db, "r+"); //abrir para lectura y escritura);
  char userFormat[MAX_LINE];
  sprintf(userFormat, ":::%s\n", user);
  int userLine = searchUserPos(fd, userFormat);
  printf("userLine %d\n", userLine);
  *lastLine = searchNextUserPosWithoutFD(fd, userLine);
  char str[MAX_FILE_LINE];
  int line = 0;

  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if (line == userLine + 1 ) {
      if(str[0] == '-' && str[1] == '>'){
        *firstLine = line + 1;
      }
    }else if(line == userLine + 2 ) {
      if(str[0] == '-' && str[1] == '>'){
        *firstLine = line ;
      }
    }
  }

  fclose(fd);
}//end findContentUser



//mira si está conectado un usuario y si lo está devuelve la línea en la que está, si no existe -1.Devuelve 0 si existe, pero no está conectado
int isConnectedWithoutFD(FILE* fd, char *userFormat){
  unsigned long position = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  char str[MAX_FILE_LINE];
  int line = 0;
  bool found = false;
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    // printf("connected func: %s\n", str);
    if(found == true){ //si se le encuentra, en la siguiente línea se ve si empieza por $ (que indica que es una línea ip:port)
      // printf("%s %c\n", userFormat, str[0] );
      if(str[0] == '$'){
        fseek(fd, position, SEEK_SET);
        return line-1;
      }
      else{
        fseek(fd, position, SEEK_SET);
        return 0;
      }
    }
    if (strcmp(str, userFormat) == 0)
      found = true;
  }
  fseek(fd, position, SEEK_SET);
  return -1; //not connected
}


//mira si está conectado un usuario y si lo está devuelve la línea en la que está, si no existe -1.Devuelve 0 si existe, pero no está conectado
int isConnected(FILE* fd, char *userFormat){
  char str[MAX_FILE_LINE];
  int line = 0;
  bool found = false;
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if(found == true){ //si se le encuentra, en la siguiente línea se ve si empieza por $ (que indica que es una línea ip:port)
      // printf("%s %c\n", userFormat, str[0] );
      if(str[0] == '$')
        return line-1;
      else
        return 0;
    }
    if (strcmp(str, userFormat) == 0)
      found = true;
  }
  return -1; //not connected
}
//Comprueba si está el fichero, si estaba ya registrado para ese usuario
int searchFile(FILE* fd, char *file, int userLine, int nextUserLine){
  fseek(fd, 0, SEEK_SET); //se pone el puntero del fichero al principio
  char fileFormat[MAX_FILE_LINE];
  sprintf(fileFormat, "->%s", file);
  char str[MAX_FILE_LINE];
  char strCopy [MAX_FILE_LINE];
  int line = 0;
  //buscamos en el fichero
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if (line > userLine && line < nextUserLine){ //si está en la zona del usuario, se compruebas sus ficheros
      strcpy(strCopy,str);
      char *ptr = strtok(str, "||");
      if(strcmp(fileFormat, ptr) == 0) //si encuentra el mismo fichero para el usuario devuelve 1
        return line;
    }
  }
  return 0; //si no se encuentra el fichero para el usuario devuelve 0
}

//Añade el archivo y la descripción
int addFile(FILE* fd, char *fileFormat, int userLine, int nextUserLine){
  int line = 0;
  FILE *fd2;
  char str[MAX_FILE_LINE], temp[] = "temp.txt";

  if (nextUserLine == -1){
    fseek(fd, 0, SEEK_END); //se pone el puntero del fichero al final
    fprintf(fd, "%s", fileFormat);
  }else{
    fseek(fd, 0, SEEK_SET); //se pone el puntero del fichero al principio
    fd2 = fopen(temp, "w"); // abrir un archivo temporal para escribir
    if (fd2 == NULL)
      return 4; //devuelve error
    // printf("%d\n", nextUserLine);
    while (!feof(fd)) {
      strcpy(str, "\0");
      fgets(str, MAX_FILE_LINE, fd);
      if (!feof(fd)){
        line++;
        if (line == nextUserLine){
           fprintf(fd2, "%s", fileFormat);
        }
        fprintf(fd2, "%s", str);
      }
    }
    fclose(fd2); //cerramos el fichero auxiliar
  }
  //cerramos todos los ficheros y actualizamos el fichero
  fclose(fd);
  remove(DATABASE_NAME);  		// remove the original file
  rename(temp, DATABASE_NAME); 	// rename the temporary file to original name

  return 0; //todo fue bien
} //end of addFile

//Elimina archivo
int deleteFile(FILE* fd, int fileLine){
  int line = 0;
  FILE *fd2;
  char str[MAX_FILE_LINE], temp[] = "temp.txt";

  fseek(fd, 0, SEEK_SET); //se pone el puntero del fichero al principio
  fd2 = fopen(temp, "w"); // abrir un archivo temporal para escribir
  if (fd2 == NULL)
    return 4; //devuelve error

  while (!feof(fd)) {
    strcpy(str, "\0");
    fgets(str, MAX_FILE_LINE, fd);
    if (!feof(fd)){
      line++;
      if (line < fileLine || line > fileLine){
         fprintf(fd2, "%s", str);
      }
    }
  }
  //cerramos todos los ficheros y actualizamos el fichero
  fclose(fd2); //cerramos el fichero auxiliar
  fclose(fd);
  remove(DATABASE_NAME);  		// remove the original file
  rename(temp, DATABASE_NAME); 	// rename the temporary file to original name
  return 0; //todo fue bien
} //end of addFile


//Buscar línea del user
int searchUserPos(FILE* fd, char *userFormat){
  unsigned long position = ftell(fd);
  fseek(fd, 0, SEEK_SET);
  char str[MAX_FILE_LINE];
  int line = 0;
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if (strcmp(str, userFormat) == 0){
      fseek(fd, position, SEEK_SET);
      return line;
    }
  }
  fseek(fd, position, SEEK_SET);
  return -1;
}

//busca la línea en la que está el siguiente al pasado por parámetro
int searchNextUserPosWithoutFD(FILE* fd, int userLine){ //devuelve -1 si no hay más usuarios, y en caso contrario la posición del siguiente
  unsigned long position = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  char str[MAX_FILE_LINE];
  int line = 0;
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if(line > userLine){
      if (str[0] == ':' && str[1] == ':' && str[2] == ':'){
        fseek(fd, position, SEEK_SET);
        return line;
      }
    }
  }
  fseek(fd, position, SEEK_SET);
  return -1;
}


//busca la línea en la que está el siguiente al pasado por parámetro
int searchNextUserPos(FILE* fd, int userLine){ //devuelve -1 si no hay más usuarios, y en caso contrario la posición del siguiente
  fseek(fd, 0, SEEK_SET);
  char str[MAX_FILE_LINE];
  int line = 0;
  while(fgets(str, sizeof(str), fd)) {
    line ++;
    if(line > userLine){
      if (str[0] == ':' && str[1] == ':' && str[2] == ':'){
        return line;
      }
    }
  }
  return -1;
}

//función para borrar a un usuario y sus canciones
int deleteUser(FILE* fd, int userLine){
  int nextUserLine = searchNextUserPos(fd, userLine);
  // printf("nextUserLine %d\n", nextUserLine);
  int ctr = 0;
  FILE *fd2;
  char str[MAX_FILE_LINE], temp[] = "temp.txt";
  fd2 = fopen(temp, "w"); // abrir un archivo temporal para escribir
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
  //cerramos todos los ficheros y actualizamos el fichero
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

void serverMsg(char * msg){
  printf("%s\n", msg);
  printf("s> ");
  fflush( stdout );
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
