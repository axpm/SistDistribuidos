import java.io.*;
import java.lang.* ;
import java.net.* ;
import java.util.* ;
import gnu.getopt.Getopt;

class Pair {
  public String ip;
  public String port;

  public Pair(String i1, String i2){
   ip = i1;
   port = i2;
  }

}


class myThread extends Thread {

	ServerSocket serverAddr;
	int serverPort;
	String user;
	// to stop the thread
  private boolean exit;


	public myThread(String serverPort, String user) {
		this.serverAddr = null;
		this.serverPort = Integer.parseInt(serverPort);
		this.user = user;
	}
	public void run(){  //se lanza el servidor que escuchará las peticiones
		// lanzará hilos de procesar peticiones, no será necesario si se hace un servidor secuencial
		//si se copia solo de uno en uno, entonces no será necesario crear más subhilos
		try {
			this.serverAddr = new ServerSocket(this.serverPort);
		}
		catch (Exception e){
			System.err.println("Error creando socket");
			// if (serverAddr != null){
			// serverAddr.close();
			// }
			// return;
		}

		while (!exit){
			try {
				Socket client = serverAddr.accept();

				//cada vez que se acepta una conexión, se envían los datos a un hilo
				// new ProcessRequest(client).start();
				//ahora con solo un hilo

				// InputStream istream = client.getInputStream();
				// ObjectInput in = new ObjectInputStream(istream);

				DataInputStream in = new DataInputStream(client.getInputStream()); //Recibir
				DataOutputStream out = new DataOutputStream(client.getOutputStream());//Enviar

				String operation = readFromServer(in);
				String remoteFile = readFromServer(in);
				if(operation.equals("GET_FILE")){

				}else{
					out.writeBytes("2"); //Escribimos en la salida del cliente el error al cliente
					out.write('\0'); // inserta el código ASCII 0 al final
				}

				//comprobar que el usuario tiene este archivo en sus archivos
				if(checkAvailability(remoteFile, this.user) ){ //si está disponible, se enviará
					//leer del archivo y enviarlo al cliente

					//objetos para leer
					FileReader fr = null;
					BufferedReader brf = null;

					try{
						//leer archivo remoto
						String rf = "/temp" + remoteFile;
						fr = new FileReader(rf);
						brf = new BufferedReader(fr);

					}catch (Exception e) {
						out.writeBytes("2"); //Escribimos en la salida del cliente el error al cliente
						out.write('\0'); // inserta el código ASCII 0 al final
						//cerrar archivos en caso de error
						if( fr != null ){
					      fr.close();
					    }
					}
					// Lectura del fichero
					String strLine;
					//hasta que lea nulo, escribe en el archivo local
					while( (strLine = brf.readLine()) !=null){
					 out.writeBytes(strLine); //Enviamos lo que se lee
					}
					out.write('\0'); // inserta el código ASCII 0 al final

					//cerramos archivos
					fr.close();

				}else{
					out.writeBytes("2"); //Escribimos en la salida del cliente el error al cliente
					out.write('\0'); // inserta el código ASCII 0 al final
				}

				client.close(); //cerramos la conexión con el cliente
			}catch(Exception e) {
				// out.writeBytes("2"); //Escribimos en la salida del cliente el error al cliente
				// out.write('\0'); // inserta el código ASCII 0 al final
			}

			// client.close(); //cerramos la conexión con el cliente
		}//end whileTrue
		try {
			serverAddr.close(); //cerramos el servidor
		} catch(Exception e) {
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
		}
	}



	/*********** READ FROM SERVER A STRING *********/
	private static String readFromServer(DataInputStream in) throws Exception{
		String message = "";
		byte[] ch = new byte[1];
		try {
			//leer un String
			do{
				ch[0] = in.readByte(); //Leemos la respuesta
				if (ch[0] != '\0'){
					String d = new String(ch);
					message = message + d; //Concatenamos
				}
			}while(ch[0] != '\0');
		}catch (Exception e) {
			message = "";
			return message;
		}
		return message;

	}

	private static boolean checkAvailability(String file, String user){
		//hacer el protocolo para listar los archivos de un usuario


		return true; //si estaba disponible
	}

}//end of class myThread

// class ProcessRequest extends Thread {
// 	private Socket sc;
//
//
// 	public ProcessRequest(Socket s) {
// 		sc = s;
// 	}
//
// 	public void run() { //función que se realiza para tratar la petición que llega
// 		String remoteFile;
// 		// TODO: código de copiar el archivo
//
// 		//leer del cliente y copiar remoteFile y localFile, para saber de dónde copiar y a donde copiar
// 		// if (cont) {
// 		//
// 		// //se abre el archivo local para escribir y el remoto para escribir
// 		// //objetos para escribir
// 		// FileWriter fw = null;
// 		// PrintWriter pw = null;
// 		// //objetos para leer
// 		// FileReader fr = null;
// 		// BufferedReader brf = null;
// 		//
// 		// try{
// 		// 	//leer archivo remoto
// 		// 	String rf = "/temp" + remote_file_name;
// 		// 	fr = new FileReader(rf);
// 		// 	brf = new BufferedReader(fr);
// 		// 	//escribir archivo local
// 		// 	String lf = "/temp" + local_file_name;
// 		// 	fw = new FileWriter(lf);
// 		// 	pw = new PrintWriter(fw);
// 		//
// 		// }catch (Exception e) {
// 		// 	System.out.println("c> GET_FILE FAIL");
// 		// 	//cerrar archivos en caso de error
// 		// 	if( fr != null ){
// 		//        fr.close();
// 		//     }
// 		// 	if( fw != null ){
// 		//        fw.close();
// 		//     }
// 		// 	return -1;
// 		// }
// 		// // Lectura del fichero
// 		// String StrLine;
// 		// //hasta que lea nulo, escribe en el archivo local
// 		// while( (StrLine = brf.readLine()) !=null){
// 		// 	 pw.println(StrLine); //escribir en el local
// 		// }
// 		//
// 		// //cerramos archivos
// 		// fr.close();
// 		// fw.close();
// 	// }// fin de continue
// 	// sc.close(); //cerrar socket
// 		try {
// 			sc.close(); //close socket before leaving
//
// 		} catch(Exception e) {
//
// 		}
// 	}
//
// }//end of class ProcessRequest


class client {


	/******************* ATTRIBUTES *******************/

	private static String _server   = null;
	private static int _port = -1;
	private static final int MAX_LINE = 256;


	//no seguro
	/************************************ OJO  ***************************************/
	private static myThread _th = null;
	private static boolean userConnected = false;
	private static String userOperating = null;

	/********************* METHODS ********************/

	/**
	 * @param user - User name to register in the system
	 *
	 * @return ERROR CODE
	 */
	static int register(String user) throws java.io.IOException{
		//chequear que no empieza por :::
		if(!checkUserName(user)){
			System.out.println("c> REGISTER FAIL");
			return -1;
		}

		Socket sc = null;
		try {
			// Crear las conexiones
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "REGISTER";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send user
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final


			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];

			switch(c) {
				case '0':
					System.out.println("c> REGISTER OK");
					break;
				case '1':
					System.out.println("c> USERNAME IN USE");
					break;
				default:
					System.out.println("c> REGISTER FAIL");
				}

			sc.close();

		} catch(Exception e) {
			System.out.println("c> REGISTER FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}

		return 0;
	}

	/**
	 * @param user - User name to unregister from the system
	 *
	 * @return ERROR CODE
	 */
	static int unregister(String user) throws java.io.IOException{
		//chequear que no empieza por :::
		if(!checkUserName(user)){
			System.out.println("c> UNREGISTER FAIL");
			return -1;
		}

		Socket sc = null;
		try {
			// Crear las conexiones
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "UNREGISTER";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send user
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];

			switch(c) {
				case '0':
					System.out.println("c> UNREGISTER OK");
					break;
				case '1':
					System.out.println("c> USER DOES NOT EXIST");
					break;
				default:
					System.out.println("c> UNREGISTER FAIL");
				}

			sc.close();
		} catch(Exception e) {
			System.out.println("c> UNREGISTER FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}

		return 0;
	}

    	/**
	 * @param user - User name to connect to the system
	 *
	 * @return ERROR CODE
	 */
	static int connect(String user) throws java.io.IOException{
		//
		// System.out.println("CONNECT " + user);

		//para elegir un puerto libre, asignar el 0 y elegirá uno libre
		//findRandomOpenPortOnAllLocalInterfaces(); //esta funcion te hace eso ya

		//crear hilo y crear ServerSocket(0)

		//enviar mensaje

		//recibir byte error


		// -------------------------------HECHO DE FORMA SECUENCIAL ----////////////////

		//chequear que no empieza por :::
		if(!checkUserName(user)){
			System.out.println("c> CONNECT FAIL");
			return -1;
		}

		// Crear las conexiones
		Socket sc = null;
		try {
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "CONNECT";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send user
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send port
			message = "" + findRandomOpenPort(); //envía un puerto libre
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];
			boolean cont = false;

			switch(c) {
				case '0':
					System.out.println("c> CONNECT OK");
					userConnected = true;
					userOperating = user;
					break;
				case '1':
					System.out.println("c> CONNECT FAIL, USER DOES NOT EXIST");
					break;
				case '2':
					System.out.println("c> USER ALREADY CONNECTED");
					break;
				default:
					System.out.println("c> CONNECT FAIL");
				}

			sc.close();
			//buscar puerto del cliente
			if (userConnected) {
				//iniciamos un servidor de escucha
				Pair ipPort = new Pair(null, null);
				_th = new myThread(ipPort.ip, userOperating);
				_th.start();
			}

		} catch(Exception e) {
			System.out.println("c> CONNECT FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}




		return 0;
	}

	 /**
	 * @param user - User name to disconnect from the system
	 *
	 * @return ERROR CODE
	 */
	static int disconnect(String user) throws java.io.IOException{
		//chequear que no empieza por :::
		if(!checkUserName(user)){
			System.out.println("c> DISCONNECT FAIL");
			return -1;
		}

		Socket sc = null;
		try {
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "DISCONNECT";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send user
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];

			switch(c) {
				case '0':
					System.out.println("c> DISCONNECT OK");
					break;
				case '1':
					System.out.println("c> DISCONNECT FAIL / USER DOES NOT EXIST");
					break;
				case '2':
					System.out.println("c> DISCONNECT FAIL / USER NOT CONNECTED");
					break;
				default:
					System.out.println("c> DISCONNECT FAIL");
				}

			sc.close();
		} catch(Exception e) {
			System.out.println("c> DISCONNECT FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}


		return 0;
	}

	 /**
	 * @param file_name    - file name
	 * @param description - description
	 *
	 * @return ERROR CODE
	 */
	static int publish(String file_name, String description) throws java.io.IOException{
		//chequear que no empieza por :::
		if(!checkFileName(file_name) || !checkDescription(description)){
			System.out.println("c> PUBLISH FAIL");
			return -1;
		}

		Socket sc = null;
		try {
			//crear la conexión
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "PUBLISH";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			String user = "user1"; //Ver cómo obtenerlo-------------------------------------------------------- IMPORTANTE
			//send user
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send file_name
			message = "" + file_name; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send description
			message = "" + description; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];

			switch(c) {
				case '0':
					System.out.println("c> PUBLISH OK");
					break;
				case '1':
					System.out.println("c> PUBLISH FAIL, USER DOES NOT EXIST");
					break;
				case '2':
					System.out.println("c> PUBLISH FAIL, USER NOT CONNECTED");
					break;
				case '3':
					System.out.println("c> PUBLISH FAIL, CONTENT ALREADY PUBLISHED");
					break;
				default:
					System.out.println("c> PUBLISH FAIL");
				}

			sc.close();
		} catch(Exception e) {
			System.out.println("c> PUBLISH FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}


		return 0;
	}

	 /**
	 * @param file_name    - file name
	 *
	 * @return ERROR CODE
	 */
	static int delete(String file_name) throws java.io.IOException{
		//chequear que no empieza por ->
		if (!checkFileName(file_name)) {
			System.out.println("c> DELETE FAIL");
			return -1;
		}

		Socket sc = null;
		try {
			//Crear conexiones
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "DELETE";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			String user = "user1"; //Ver cómo obtenerlo-------------------------------------------------------- IMPORTANTE
			//send user
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send file_name
			message = "" + file_name; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];

			switch(c) {
				case '0':
					System.out.println("c> DELETE OK");
					break;
				case '1':
					System.out.println("c> DELETE FAIL, USER DOES NOT EXIST");
					break;
				case '2':
					System.out.println("c> DELETE FAIL, USER NOT CONNECTED");
					break;
				case '3':
					System.out.println("c> DELETE FAIL, CONTENT NOT PUBLISHED");
					break;
				default:
					System.out.println("c> DELETE FAIL");
				}

			sc.close();
		} catch(Exception e) {
			System.out.println("c> DELETE FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}


		return 0;
	}

	 /**
	 * @return ERROR CODE
	 */
	static int list_users() throws java.io.IOException{

		Socket sc = null;
		try {
			//crear la conexiones
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "LIST_USERS";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			String user = "user1"; //Ver cómo obtenerlo-------------------------------------------------------- IMPORTANTE
			//send user
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];
			boolean cont = false;

			switch(c) {
				case '0':
					System.out.println("c> LIST_USERS OK");
					cont = true;
					break;
				case '1':
					System.out.println("c> LIST_USERS FAIL, USER DOES NOT EXIST");
					break;
				case '2':
					System.out.println("c> LIST_USERS FAIL, USER NOT CONNECTED");
					break;
				default:
					System.out.println("c> LIST_USERS FAIL");
			}

			//si fue todo bien se siguen enviando cosas
			if (cont) {
				//Objetos para leer por pantalla
				InputStreamReader  is = new InputStreamReader(System.in); //Leemos por pantalla
				BufferedReader br = new BufferedReader(is); //Guardamos
				//Leemos por pantalla
				System.out.print("c> ");
			  message = br.readLine();
				//enviamos el número recibido
				out.writeBytes(message); //Escribimos en la salida del cliente
				out.write('\0'); // inserta el código ASCII 0 al final

				int n;
				try {
					n = Integer.parseInt(message);
				} catch(Exception e) {
					System.out.println("c> LIST_USERS FAIL");
					return -1;
				}

				for (int i = 0; i < n ; i++) {
					//recibir user
					String usersConn = readFromServer(in);
					//recibir ip
					String ipConn = readFromServer(in);
					//recibir port
					String portConn = readFromServer(in);

					if( usersConn != "" && usersConn != " "){ //si no está vacío se imprime
						System.out.println(" " + usersConn + " " + ipConn + " " + portConn);
					}
				}

			}// fin de continue

			sc.close();
		} catch(Exception e) {
			System.out.println("c> LIST_USERS FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}



		return 0;
	}


	 /**
	 * @param user_name    - user name
	 *
	 * @return ERROR CODE
	 */
	static int list_content(String user_name) throws java.io.IOException{
		//chequear que no empieza por :::
		if(!checkUserName(user_name)){
			System.out.println("c> LIST_CONTENT FAIL");
			return -1;
		}

		Socket sc = null;
		try {
			//Crear la conexión
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			//send operation
			String message = "LIST_CONTENT";
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			String user = "user1"; //Ver cómo obtenerlo-------------------------------------------------------- IMPORTANTE
			//send user que está haciendo la operación
			message = "" + user; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final
			//send user del que se quiere saber la info
			message = "" + user_name; //user;// // mensaje más código ASCII 0 al final
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];
			boolean cont = false;

			switch(c) {
				case '0':
					System.out.println("c> LIST_CONTENT OK");
					cont = true;
					break;
				case '1':
					System.out.println("c> LIST_CONTENT FAIL, USER DOES NOT EXIST");
					break;
				case '2':
					System.out.println("c> LIST_CONTENT FAIL, USER NOT CONNECTED");
					break;
				default:
					System.out.println("c> LIST_CONTENT FAIL");
			}

			//si fue todo bien se siguen enviando cosas
			if (cont) {
				//Objetos para leer por pantalla
				InputStreamReader  is = new InputStreamReader(System.in); //Leemos por pantalla
				BufferedReader br = new BufferedReader(is); //Guardamos
				//Leemos por pantalla
				System.out.print("c> ");
			  message = br.readLine();
				//enviamos el número recibido
				out.writeBytes(message); //Escribimos en la salida del cliente
				out.write('\0'); // inserta el código ASCII 0 al final

				int n;
				try {
					n = Integer.parseInt(message);
				} catch(Exception e) {
					System.out.println("c> LIST_CONTENT FAIL");
					return -1;
				}

				for (int i = 0; i < n ; i++) {
					//recibir user
					String file = readFromServer(in);

					if( file != "" && file != " "){ //si no está vacío se imprime
						System.out.println(" " + file);

					}
				}

			}// fin de continue

			sc.close();
		} catch(Exception e) {
			System.out.println("c> LIST_CONTENT FAIL");
			if( sc != null ){
				sc.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}



		return 0;
	}

	 /**
	 * @param user_name    - user name
	 * @param remote_file_name    - remote file name
	 * @param local_file_name  - local file name
	 *
	 * @return ERROR CODE
	 */
	static int get_file(String user_name, String remote_file_name, String local_file_name) throws java.io.IOException{

		//se comprueba que los campos son correctos
		if(!checkUserName(user_name) || !checkFileName(remote_file_name) || !checkFileName(local_file_name)){
			System.out.println("c> GET_FILE FAIL");
			return -1;
		}


		String port = null;
		String server = null;
		Socket client = null;
		try {

			if(userConnected){
				//se abre un proceso para escuchar
				Pair ipPort = getPortFromUser(user_name); //Coge el puerto del server

				client = new Socket(ipPort.ip, Integer.parseInt(ipPort.port)); // creamos una conexión con el otro cliente
				DataOutputStream out = new DataOutputStream(client.getOutputStream()); //Enviar
				DataInputStream in = new DataInputStream(client.getInputStream()); //Recibir

				//send operation
				String message = "GET_FILE";
				out.writeBytes(message); //Escribimos en la salida del cliente
				out.write('\0'); // inserta el código ASCII 0 al final

				//send file a descargar
				message = "" + remote_file_name; // // mensaje más código ASCII 0 al final
				out.writeBytes(message); //Escribimos en la salida del cliente
				out.write('\0'); // inserta el código ASCII 0 al final

				//recibimos respuesta que es un solo byte
				byte[] ch = new byte[1];
				ch[0] = in.readByte(); //Leemos la respuesta
				char c = (char) ch[0];
				boolean cont = false;

				switch(c) {
					case '0':
					System.out.println("c> GET_FILE OK");
					cont = true;
					break;
					case '1':
					System.out.println("c> GET_FILE FAIL / FILE DOES NOT EXIST");
					break;
					default:
					System.out.println("c> GET_FILE FAIL");
				}

				//si fue todo bien se recibe el archivo
				if (cont){
					//leemos línea por línea de lo que se recibe y se escribe
					//objetos para escribir
					FileWriter fw = null;
					PrintWriter pw = null;

					//escribir archivo local
					String lf = "/temp" + local_file_name;
					fw = new FileWriter(lf);
					pw = new PrintWriter(fw);

					String strLine = null;
					while( (strLine = readFromServer(in) ) !=null){
						pw.println(strLine); //escribir en el local
					}
					fw.close();//cerramos el fichero local
				}

			}else{

				System.out.println("c> GET_FILE FAIL");
				return -1;

			}




		} catch(Exception e) {
			System.out.println("c> GET_FILE FAIL");
			if( client != null ){
				client.close();
			}
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}


		return 0;
	}


	/**
	 * @brief Command interpreter for the client. It calls the protocol functions.
	 */
	static void shell()
	{
		boolean exit = false;
		String input;
		String [] line;
		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

		while (!exit) {
			try {
				System.out.print("c> ");
				input = in.readLine();
				line = input.split("\\s");

				if (line.length > 0) {
					/*********** REGISTER *************/
					if (line[0].equals("REGISTER")) {
						if  (line.length == 2) {
							register(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: REGISTER <userName>");
						}
					}

					/********** UNREGISTER ************/
					else if (line[0].equals("UNREGISTER")) {
						if  (line.length == 2) {
							unregister(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: UNREGISTER <userName>");
						}
                    			}

    			/************ CONNECT *************/
    			else if (line[0].equals("CONNECT")) {
						if  (line.length == 2) {
							connect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: CONNECT <userName>");
                    				}
                    			}

    			/********** DISCONNECT ************/
    			else if (line[0].equals("DISCONNECT")) {
						if  (line.length == 2) {
							disconnect(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DISCONNECT <userName>");
                    				}
                    			}

    			/************** PUBLISH **************/
    			else if (line[0].equals("PUBLISH")) {
						if  (line.length >= 3) {
							// Remove first two words
							//String description = input.substring(input.indexOf(' ')+1).substring(input.indexOf(' ')+1);
							String description = input.substring(input.indexOf(' ')+1);
							description = description.substring(description.indexOf(' ')+1);
							publish(line[1], description); // file_name = line[1]
						} else {
							System.out.println("Syntax error. Usage: PUBLISH <file_name> <description>");
                    				}
                    			}

    			/************ DELETE *************/
    			else if (line[0].equals("DELETE")) {
						if  (line.length == 2) {
							delete(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: DELETE <file name>");
                    				}
                    			}

    			/************** LIST_USERS **************/
    			else if (line[0].equals("LIST_USERS")) {
						if  (line.length == 1) {
							// Remove first two words
							list_users();
						} else {
							System.out.println("Syntax error. Usage: LIST_USERS ");
                    				}
                    			}

    			/************ LIST_CONTENT *************/
    			else if (line[0].equals("LIST_CONTENT")) {
						if  (line.length == 2) {
							list_content(line[1]); // userName = line[1]
						} else {
							System.out.println("Syntax error. Usage: LIST_CONTENT <user name>");
                    				}
                    			}

    			/************** GET_FILE **************/
    			else if (line[0].equals("GET_FILE")) {
						if  (line.length == 4) {
							get_file(line[1], line[2], line[3]);
						} else {
							System.out.println("Syntax error. Usage: GET_FILE <user> <remote_file_name> <local_file_name>");
                    				}
                    			}


    			/************** QUIT **************/
    			else if (line[0].equals("QUIT")){
						if (line.length == 1) {
							exit = true;
						} else {
							System.out.println("Syntax error. Use: QUIT");
						}
					}

					/************* UNKNOWN ************/
					else {
						System.out.println("Error: command '" + line[0] + "' not valid.");
					}
				}
			} catch (java.io.IOException e) {
				System.out.println("Exception: " + e);
				e.printStackTrace();
			}
		}
	}

	/**
	 * @brief Prints program usage
	 */
	static void usage()
	{
		System.out.println("Usage: java -cp . client -s <server> -p <port>");
	}

	/**
	 * @brief Parses program execution arguments
	 */
	static boolean parseArguments(String [] argv)
	{
		Getopt g = new Getopt("client", argv, "ds:p:");

		int c;
		String arg;

		while ((c = g.getopt()) != -1) {
			switch(c) {
				//case 'd':
				//	_debug = true;
				//	break;
				case 's':
					_server = g.getOptarg();
					break;
				case 'p':
					arg = g.getOptarg();
					_port = Integer.parseInt(arg);
					break;
				case '?':
					System.out.print("getopt() returned " + c + "\n");
					break; // getopt() already printed an error
				default:
					System.out.print("getopt() returned " + c + "\n");
			}
		}

		if (_server == null)
			return false;

		if ((_port < 1024) || (_port > 65535)) {
			System.out.println("Error: Port must be in the range 1024 <= port <= 65535");
			return false;
		}

		return true;
	}



	/********************* MAIN **********************/

	public static void main(String[] argv)
	{
		if(!parseArguments(argv)) {
			usage();
			return;
		}

		shell();
		System.out.println("+++ FINISHED +++");
	}


	/*********************** OTHERS ***************************/
	// private static String getPortFromUser(String userName, Socket sc) throws java.io.IOException{
	private static Pair getPortFromUser(String userName) throws java.io.IOException{

		String ip = null;
		String port = null;
		int n = 15;
		Socket sc = null;
		try {
			//Crear la conexión
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

			while(port != null){

				//send operation
				String message = "LIST_USERS";
				out.writeBytes(message); //Escribimos en la salida del cliente
				out.write('\0'); // inserta el código ASCII 0 al final

				//send userOperating
				message = "" + userOperating; //user;// // mensaje más código ASCII 0 al final
				out.writeBytes(message); //Escribimos en la salida del cliente
				out.write('\0'); // inserta el código ASCII 0 al final

				//recibimos respuesta que es un solo byte
				byte[] ch = new byte[1];
				ch[0] = in.readByte(); //Leemos la respuesta
				char c = (char) ch[0];
				boolean cont = false;

				switch(c) {
					case '0':
						cont = true;
						break;
					default:
						cont = false;
				}

				//si fue todo bien se siguen enviando cosas
				if (cont) {

					for (int i = 0; i < n ; i++) {
						//recibir user
						String usersConn = readFromServer(in);
						//recibir ip
						String ipConn = readFromServer(in);
						//recibir port
						String portConn = readFromServer(in);

						if( usersConn.equals(userName)){ // si usuario que se saca es el correcto, se obtiene su puerto
							port = portConn;
							ip = ipConn;
							// return port;
						 return new Pair(ip, port);
						}
					}

				}// fin de continue

			n *= 2; //si no tiene el puerto, para la siguiente ronda, sacará el doble de usuarios
			cont = false;

				}//fin del while
			} catch(Exception e) {
				// System.out.println("c> LIST_USERS FAIL");

 				return new Pair(ip, port);
			}


	 return new Pair(ip, port);
	}





	private static Integer findRandomOpenPort() throws IOException {
    try (
        ServerSocket socket = new ServerSocket(0);
    ) {
      return socket.getLocalPort();
    }
  }

	private static boolean checkUserName(String user){
		if(user.length() <= MAX_LINE){
			if (user.length() > 2){
				if (user.charAt(0) == ':' && user.charAt(1) == ':' && user.charAt(2) == ':' )
					return false; //no puede empezar por :::loQueSea
				return true;
			}else{ //tiene menos de tres caracteres
				return true;
			}
		}else return false; //se pasa de longitud
	}


	private static boolean checkFileName(String file){
		if(file.length() <= MAX_LINE){
			if (file.length() > 1){
				if (file.charAt(0) == '-' && file.charAt(1) == '>')
					return false; //no puede empezar por :::loQueSea
				return true;
			}else{ //tiene menos de dos caracteres
				return true;
			}
		}else return false; //se pasa de longitud
	}


	private static boolean checkDescription(String desc){
		if(desc.length() <= MAX_LINE){
			if (desc.length() > 1){
				if (desc.charAt(0) == '|' && desc.charAt(1) == '|' )
					return false; //no puede empezar por :::loQueSea
				return true;
			}else{ //tiene menos de dos caracteres
				return true;
			}
		}else return false; //se pasa de longitud
	}

	/*********** READ FROM SERVER A STRING *********/
	private static String readFromServer(DataInputStream in) throws Exception{
		String message = "";
		byte[] ch = new byte[1];
		try {
			//leer un String
			do{
				ch[0] = in.readByte(); //Leemos la respuesta
				if (ch[0] != '\0'){
					String d = new String(ch);
					message = message + d; //Concatenamos
				}
			}while(ch[0] != '\0');
		}catch (Exception e) {
			message = "";
			return message;
		}
		return message;

	}

}//fin de clase
