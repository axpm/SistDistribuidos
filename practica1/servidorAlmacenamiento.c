/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "storage.h"
#include "imple.h"

bool_t
registeruser_1_svc(char *user, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	*result = registerUserImple(user); //función desarrollada en imple.c
	retval = TRUE;

	return retval;
}

bool_t
unregisteruser_1_svc(char *user, int *result, struct svc_req *rqstp)
{
	bool_t retval;

	*result = unregisterUserImple(user);
	retval = TRUE;

	return retval;
}

bool_t
publish_1_svc(char *user, char *file, char *desc, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	*result =	publishImple(user, file, desc);
	retval = TRUE;

	return retval;
}

bool_t
deletecontent_1_svc(char *user, char *file, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	*result = deleteContentImple(user, file);
	retval = TRUE;

	return retval;
}

bool_t
connectuser_1_svc(char *user, char *ip, char *port, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	*result = connectUserImple(user, ip, port);
	retval = TRUE;

	return retval;
}

bool_t
disconnectuser_1_svc(char *user, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	*result = disconnectUserImple(user);
	retval = TRUE;

	return retval;
}

bool_t
check_list_users_1_svc(char *user, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	*result = list_usersImple(user);
	retval = TRUE;

	return retval;
}

bool_t
listconnectedusers_1_svc(t_listUsers *result, struct svc_req *rqstp)
{
	bool_t retval;

	bool noMore = false;
	int userLine = 0, nextUserLine = 0;
	char user[MAX_LINE];
	char ip[MAX_LINE];
	char port[MAX_LINE];
	t_listUsers aux;

	*result = NULL;
	// for (int i = 0; i < n; i++){
	while(!noMore){
		fillUserInfoImple(user, ip, port, &userLine, &nextUserLine, &noMore);

		//creando un nuevo item
		aux = (t_listUsers) malloc(sizeof(listUsers));
		aux->user.user_len = 256;
		aux->ip.ip_len = 256;
		aux->port.port_len = 256;
		aux->user.user_val = (char *)malloc(256);
		aux->ip.ip_val = (char *)malloc(256);
		aux->port.port_val = (char *)malloc(256);

		// if (noMore){ //envío de relleno
		// 	// sprintf(aux->user.user_val, "%s", "PRUEBA");
		// 	sprintf(aux->user.user_val, "%c", '\0');
		// 	sprintf(aux->ip.ip_val, "%c", '\0');
		// 	sprintf(aux->port.port_val, "%c", '\0');
		// }else{ //envío normal de la info de los usuarios
		// 	// sprintf(aux->user.user_val, "%s", "PRUEBA");
		// 	sprintf(aux->user.user_val, "%s", user);
		// 	sprintf(aux->ip.ip_val, "%s", ip);
		// 	sprintf(aux->port.port_val, "%s", port);
		// }
		if(!noMore){
			sprintf(aux->user.user_val, "%s", user);
			sprintf(aux->ip.ip_val, "%s", ip);
			sprintf(aux->port.port_val, "%s", port);
			//se añade el auxiliar a la lista
			aux->next = *result;
			*result = aux;
		}
	}
	retval = TRUE;

	return retval;
}

bool_t
check_list_content_1_svc(char *user, char *userTarget, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	*result = list_contentImple(user, userTarget);
	retval = TRUE;

	return retval;
}

bool_t
list_content_1_svc(char *userTarget, t_list *result,  struct svc_req *rqstp)
{
	bool_t retval;

	bool noMore = false;
	int firstLine = 0, lastLine = 0;

	findContentUser(userTarget, &firstLine, &lastLine);

	char file[MAX_LINE];
	t_list aux;

	// for (int i = 0; i < n; i++){
	while(!noMore){
		fillContentUserImple(file, firstLine, lastLine, &noMore);
		aux = (t_list) malloc(sizeof(list));
		aux->file.file_len = 256;
		aux->file.file_val = (char *)malloc(256);

		// if (noMore){ //envío de relleno
		// 	sprintf(aux->file.file_val, "%c", '\0');
		// }else{ //envío normal de la info
		// 	sprintf(aux->file.file_val, "%s", file);
		// }
		if(!noMore){ //para evitar los blancos en la última
			sprintf(aux->file.file_val, "%s", file);
			firstLine++;
			//se añade el auxiliar a la lista
			aux->next = *result;
			*result = aux;
		}

	}
	retval = TRUE;

	return retval;
}

int
storage_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
