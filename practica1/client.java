import java.io.*;
import java.lang.* ;
import java.net.* ;
import java.util.* ;
import gnu.getopt.Getopt;

//para el apartado 3
import client.UpperServiceService;
import client.UpperService;

//Clase de apoyo para obtener ip-puerto
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
  //Para conectarse y recopilar info del servidor Central
  // private String infoServer;
  // private int infoPort;
	// to stop the thread
  private boolean exit = false;


	public myThread(int serverPort, String user){ // String infoServer, int infoPort) {
		this.serverAddr = null;
		this.serverPort = serverPort;
		this.user = user;
    // this.infoServer = infoServer;
    // this.infoPort = infoPort;
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

        //se crean las estructuras para enviar y recibir datos
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
        String rf = "/tmp/" + remoteFile;
        File tempFile = new File(rf);
        boolean available = tempFile.exists(); //comprueba que existe rf en el directorio indicado

        if(available){ //si está disponible --> se leerá del archivo y enviará al cliente

          out.writeBytes("0"); //Escribimos en la salida del cliente el error al cliente
          out.write('\0'); // inserta el código ASCII 0 al final

					//objetos para leer

          InputStream inputStream = null;

					try{
						//leer archivo remoto
            File infile = new File(rf);
            inputStream = new FileInputStream(rf);

					}catch (Exception e) {
            System.err.println("excepcion " + e.toString() );
            e.printStackTrace() ;
						out.writeBytes("2"); //Escribimos en la salida del cliente el error al cliente
						out.write('\0'); // inserta el código ASCII 0 al final
						//cerrar archivos en caso de error
						// if( fr != null ){
					  //     fr.close();
					  //   }
					}
					// Lectura del fichero
          byte[] buffer = new byte[1024];
          int length = 0;

          // while ((length = brf.read(buffer)) > 0) {
          while((length = inputStream.read(buffer)) > 0) {
            out.write(buffer, 0, length);
          }

					//cerramos archivos
          inputStream.close();
          out.close();
					client.close(); //cerramos la conexión con el cliente
          // System.out.println("llega acá");

				}else{
				  out.writeBytes("2"); //Escribimos en la salida del cliente el error al cliente
				  out.write('\0'); // inserta el código ASCII 0 al final
					client.close(); //cerramos la conexión con el cliente
				}

			}catch(Exception e) {
        // System.err.println("excepcion " + e.toString() );
        // e.printStackTrace() ;
				// out.writeBytes("2"); //Escribimos en la salida del cliente el error al cliente
				// out.write('\0'); // inserta el código ASCII 0 al final
        if (exit) {
          try {
            serverAddr.close(); //cerramos el servidor
          } catch(Exception exc) {
            // System.err.println("excepcion " + e.toString() );
            // e.printStackTrace() ;
          }
        }
			}

      // client.close(); //cerramos la conexión con el cliente
		}//end whileTrue


		try {
			serverAddr.close(); //cerramos el servidor
		} catch(Exception e) {
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
		}

    // System.out.println("FIN DE HILO"); //borrar cuando se consiga
	}//end of run

	public void finish(){
    exit = true;
    //cierra el servidor por la fuerza y no me convence
    try {
			serverAddr.close(); //cerramos el servidor
		} catch(Exception e) {
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
		}


  }



	/*********** READ FROM SERVER A STRING *********/
	private String readFromServer(DataInputStream in) throws Exception{
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



}//end of class myThread


class client {


	/******************* ATTRIBUTES *******************/

	private static String _server   = null;
	private static int _port = -1;
	private static final int MAX_LINE = 256;

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


		//chequear que no empieza por :::
		if(!checkUserName(user)){
			System.out.println("c> CONNECT FAIL");
			return -1;
		}

    //si ya estaba conectado con un usuario, da error
    if(userConnected){
      System.out.println("c> CONNECT FAIL");
      return -1;
    }

		// Crear las conexiones
		Socket sc = null;
    int port;
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
      port = findRandomOpenPort();
			message = "" + port; //envía un puerto libre
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
				// Pair ipPort = findRandomOpenPort(userOperating);
        // System.out.println(ipPort.port);
				_th = new myThread(port, userOperating);// _server, _port);
				_th.start();
			}

		} catch(Exception e) {
			System.out.println("c> CONNECT FAIL");
			if( sc != null ){
				sc.close();
			}
			System.err.println("excepcion " + e.toString() );
			e.printStackTrace() ;
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
          // userOperating = null;
          userConnected = false;
          try {
            _th.finish(); //acabar el hilo que estaba conectado
          } catch(Exception e) {

          }
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

    //transformar a mayúsculas con Servicios Web la descripción
    UpperServiceService service = new UpperServiceService();
    UpperService port = service.getUpperServicePort();
    description = port.standardizeString(description);

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

			//send user
			message = "" + userOperating; //user;// // mensaje más código ASCII 0 al final
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

			//send user
			message = "" + userOperating; //user;// // mensaje más código ASCII 0 al final
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

    if(!userConnected){
      System.out.println("c> LIST_USERS FAIL, USER NOT CONNECTED");
      return -1;
    }

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

			//send user
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
				int n;
				try {
          //recibir número de users
          String n_string = readFromServer(in);
          // System.out.println("N: " + n_string);
					n = Integer.parseInt(n_string);
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

    if(!userConnected){
			System.out.println("c> LIST_CONTENT FAIL, USER NOT CONNECTED");
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

			//send user que está haciendo la operación
			message = "" + userOperating; //user;// // mensaje más código ASCII 0 al final
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
					// System.out.println("c> LIST_CONTENT OK");
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

        int n;
				try {
          //recibir número de users
          String n_string = readFromServer(in);
          // System.out.println("N: " + n_string);
					n = Integer.parseInt(n_string);
				} catch(Exception e) {
					System.out.println("c> LIST_CONTENT FAIL");
					return -1;
				}
        System.out.println("c> LIST_CONTENT OK");
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

    String lf = "/tmp/" + local_file_name;

    File tempFile = new File(lf);
    boolean exists = tempFile.exists(); //comprueba que existe rf en el directorio indicado
    // tempFile.close();
    //si existe un archivo que se llama igual que local_file_name da error
    if(exists){
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


          File file = new File(lf);

          file.createNewFile(); //habría que controlar el error

					fw = new FileWriter(lf);
					pw = new PrintWriter(fw);

      		OutputStream outputStream = null;
          outputStream = new FileOutputStream(lf);

          byte[] buffer = new byte[1024];
          int length = 0;
          while ((length = in.read(buffer)) > 0) {
            outputStream.write(buffer, 0, length);
        	}

					fw.close();//cerramos el fichero local
          in.close();
          out.close();

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
							System.out.println("c> Syntax error. Usage: REGISTER <userName>");
						}
					}

					/********** UNREGISTER ************/
					else if (line[0].equals("UNREGISTER")) {
						if  (line.length == 2) {
							unregister(line[1]); // userName = line[1]
						} else {
							System.out.println("c> Syntax error. Usage: UNREGISTER <userName>");
						}
                    			}

    			/************ CONNECT *************/
    			else if (line[0].equals("CONNECT")) {
						if  (line.length == 2) {
							connect(line[1]); // userName = line[1]
						} else {
							System.out.println("c> Syntax error. Usage: CONNECT <userName>");
                    				}
                    			}

    			/********** DISCONNECT ************/
    			else if (line[0].equals("DISCONNECT")) {
						if  (line.length == 2) {
							disconnect(line[1]); // userName = line[1]
						} else {
							System.out.println("c> Syntax error. Usage: DISCONNECT <userName>");
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
							System.out.println("c> Syntax error. Usage: PUBLISH <file_name> <description>");
                    				}
                    			}

    			/************ DELETE *************/
    			else if (line[0].equals("DELETE")) {
						if  (line.length == 2) {
							delete(line[1]); // userName = line[1]
						} else {
							System.out.println("c> Syntax error. Usage: DELETE <file name>");
                    				}
                    			}

    			/************** LIST_USERS **************/
    			else if (line[0].equals("LIST_USERS")) {
						if  (line.length == 1) {
							// Remove first two words
							list_users();
						} else {
							System.out.println("c> Syntax error. Usage: LIST_USERS ");
                    				}
                    			}

    			/************ LIST_CONTENT *************/
    			else if (line[0].equals("LIST_CONTENT")) {
						if  (line.length == 2) {
							list_content(line[1]); // userName = line[1]
						} else {
							System.out.println("c> Syntax error. Usage: LIST_CONTENT <user name>");
                    				}
                    			}

    			/************** GET_FILE **************/
    			else if (line[0].equals("GET_FILE")) {
						if  (line.length == 4) {
							get_file(line[1], line[2], line[3]);
						} else {
							System.out.println("c> Syntax error. Usage: GET_FILE <user> <remote_file_name> <local_file_name>");
                    				}
                    			}


    			/************** QUIT **************/
    			else if (line[0].equals("QUIT")){
						if (line.length == 1) {
              try {
                if(_th != null){
                  _th.finish(); //acabar el hilo que estaba conectado
                }
              } catch(Exception e) {
              }

              // if(userOperating != null && !userOperating.equals("") && !userOperating.equals(" ")){
              //   disconnect(userOperating);
              // }

              //si estaba conectado se sale
              if(userConnected){
                disconnect(userOperating);
              }
							exit = true;
						} else {
							System.out.println("c> Syntax error. Use: QUIT");
						}
					}

					/************* UNKNOWN ************/
					else {
						System.out.println("c> Error: command '" + line[0] + "' not valid.");
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
		// int n = 15;
		Socket sc = null;
		try {
			//Crear la conexión
			sc = new Socket(_server, _port);

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

      int counter = 0;
			// while(port == null){

				//send operation
				String message = "LIST_USERS";
				out.writeBytes(message); //Escribimos en la salida del cliente
				out.write('\0'); // inserta el código ASCII 0 al final

				//send userOperating
				message = "" + userOperating; // mensaje más código ASCII 0 al final
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

        int n;
        try {
          //recibir número de users
          String n_string = readFromServer(in);
          // System.out.println("N: " + n_string);
          n = Integer.parseInt(n_string);
        } catch(Exception e) {
          System.out.println("c> LIST_USERS FAIL");
          return null;
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
              try {
                if (sc != null){
                  sc.close();
                }
              } catch(IOException ex) {

              }
						 return new Pair(ip, port);
						}
					}

				}// fin de continue
      //
			// n *= 2; //si no tiene el puerto, para la siguiente ronda, sacará el doble de usuarios
			// cont = false;
      // counter += 1;
      //
			// 	}//fin del while
			} catch(Exception e) {
				// System.out.println("c> LIST_USERS FAIL");
        try {

          if (sc != null){
            sc.close();
          }
        } catch(IOException ex) {

        }
 				return new Pair(ip, port);
			}

  try {
    if (sc != null){
      sc.close();
    }
  } catch(IOException ex) {

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
      if(user.length() > 0){ //tiene menos de dos caracteres
        if (user.charAt(0) == '$'){
          return false;
        }
        if (user.length() > 1){
          if (user.charAt(0) == '-' && user.charAt(1) == '>'){
            return false;
          }
          if (user.length() > 2){
            if (user.charAt(0) == ':' && user.charAt(1) == ':' && user.charAt(2) == ':' ){
              return false; //no puede empezar por :::loQueSea
            }
          }
        }
        return true;
      }
    }
    return false;
  }

	private static boolean checkFileName(String file){
    if(file.length() <= MAX_LINE){
      if(file.length() > 0){ //tiene menos de dos caracteres
        if (file.charAt(0) == '$'){
          return false;
        }
        if (file.length() > 1){
          if (file.charAt(0) == '-' && file.charAt(1) == '>'){
            return false;
          }
          if (file.length() > 2){
            if (file.charAt(0) == ':' && file.charAt(1) == ':' && file.charAt(2) == ':' ){
              return false; //no puede empezar por :::loQueSea
            }
          }
        }
        return true;
      }
    }
    return false;
  }


	private static boolean checkDescription(String desc){
		if(desc.length() <= MAX_LINE){
			if (desc.length() > 1){
				if (desc.charAt(0) == '|' && desc.charAt(1) == '|' )
					return false; //no puede empezar por :::loQueSea
				return true;
			}else{
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

  static void copyFile(String src, String dest) throws IOException {
    		InputStream inputStream = null;
    		OutputStream outputStream = null;
    		try {
		      File infile = new File(src);
  	    	File outfile = new File(dest);

      		inputStream = new FileInputStream(src);
      		outputStream = new FileOutputStream(dest);

      		byte[] buffer = new byte[1024];
      		int length = 0;
      		while ((length = inputStream.read(buffer)) > 0) {
          			outputStream.write(buffer, 0, length);
      		}
    		} finally {
      		inputStream.close();
      		outputStream.close();
    		}
	}

}//fin de clase
