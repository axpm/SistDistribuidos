import java.io.*;
import java.lang.* ;
import java.net.* ;
import java.util.* ;
import gnu.getopt.Getopt;


class client {


	/******************* ATTRIBUTES *******************/

	private static String _server   = null;
	private static int _port = -1;
	private static final int MAX_LINE = 256;


	/********************* METHODS ********************/

	/**
	 * @param user - User name to register in the system
	 *
	 * @return ERROR CODE
	 */
	static int register(String user)
	{
		if(user.length() > MAX_LINE ){
			System.out.println("c> REGISTER FAIL");
			return -1;
		}
		// Crear las conexiones
		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> REGISTER FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {
			// Crear las conexiones

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
	static int unregister(String user)
	{
		if(user.length() > MAX_LINE ){
			System.out.println("c> UNREGISTER FAIL");
			return -1;
		}
		// Crear las conexiones
		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> UNREGISTER FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

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
	static int connect(String user)
	{
		//
		// System.out.println("CONNECT " + user);

		//para elegir un puerto libre, asignar el 0 y elegirá uno libre
		//findRandomOpenPortOnAllLocalInterfaces(); //esta funcion te hace eso ya

		//crear hilo y crear ServerSocket(0)

		//enviar mensaje

		//recibir byte error


		// -------------------------------HECHO DE FORMA SECUENCIAL ----////////////////

		if(user.length() > MAX_LINE ){
			System.out.println("c> CONNECT FAIL");
			return -1;
		}
		// Crear las conexiones
		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> CONNECT FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

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
			message = "" + findRandomOpenPortOnAllLocalInterfaces(); //envía un puerto libre
			out.writeBytes(message); //Escribimos en la salida del cliente
			out.write('\0'); // inserta el código ASCII 0 al final

			//recibimos respuesta que es un solo byte
			byte[] ch = new byte[1];
			ch[0] = in.readByte(); //Leemos la respuesta
			char c = (char) ch[0];

			switch(c) {
				case '0':
					System.out.println("c> CONNECT OK");
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
		} catch(Exception e) {
			System.out.println("c> CONNECT FAIL");
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
	static int disconnect(String user)
	{
		if(user.length() > MAX_LINE ){
			System.out.println("c> DISCONNECT FAIL");
			return -1;
		}

		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> DISCONNECT FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

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
	static int publish(String file_name, String description)
	{
		//
		// System.out.println("PUBLISH " + file_name + " " + description);

		if(file_name.length() > MAX_LINE || description.length() > MAX_LINE ){
			System.out.println("c> PUBLISH FAIL");
			return -1;
		}

		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> PUBLISH FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

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
	static int delete(String file_name)
	{
		//
		// System.out.println("DELETE " + file_name);
		if(file_name.length() > MAX_LINE){
			System.out.println("c> DELETE FAIL");
			return -1;
		}

		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> DELETE FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

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
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}


		return 0;
	}

	 /**
	 * @return ERROR CODE
	 */
	static int list_users()
	{
		//
		// System.out.println("LIST_USERS " );
		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> LIST_USERS FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

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
	static int list_content(String user_name)
	{
		//
		// System.out.println("LIST_CONTENT " + user_name);

		if(user_name.length() > MAX_LINE){
			System.out.println("c> LIST_CONTENT FAIL");
			return -1;
		}

		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> LIST_CONTENT FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

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
	static int get_file(String user_name, String remote_file_name, String local_file_name)
	{
		// System.out.println("GET_FILE " + user_name + " "  + remote_file_name + " " + local_file_name);


		if(user_name.length() > MAX_LINE || remote_file_name.length() > MAX_LINE || local_file_name.length() > MAX_LINE){
			System.out.println("c> GET_FILE FAIL");
			return -1;
		}

		Socket sc;
		try {
			sc = new Socket(_server, _port);
		} catch(Exception e) {
			System.out.println("c> GET_FILE FAIL");
			// System.err.println("Can't reach the host.");
			// System.err.println("excepcion " + e.toString() );
			// e.printStackTrace() ;
			return -1;
		}
		try {

			DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
			DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

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
			if (cont) {

				//objetos para escribir
				FileWriter fw = null;
				PrintWriter pw = null;
				//objetos para leer
				FileReader fr = null;
				BufferedReader brf = null;
				//se abre el archivo local para escribir y el remoto para escribir
        try
        {
					//leer archivo remoto
					String rf = "/temp" + remote_file_name;
					fr = new FileReader(rf);
					brf = new BufferedReader(fr);
					//escribir archivo local
					String lf = "/temp" + local_file_name;
					fw = new FileWriter(lf);
					pw = new PrintWriter(fw);

				}catch (Exception e) {
					System.out.println("c> GET_FILE FAIL");
					//cerrar archivos en caso de error
					if( fr != null ){
               fr.close();
            }
					if( fw != null ){
               fw.close();
            }
					return -1;
				}
				// Lectura del fichero
				String StrLine;
				//hasta que lea nulo, escribe en el archivo local
				while( (StrLine = brf.readLine()) !=null){
					 pw.println(StrLine); //escribir en el local
				}

				//cerramos archivos
				fr.close();
				fw.close();

			}// fin de continue

			sc.close(); //cerrar socket
		} catch(Exception e) {
			System.out.println("c> GET_FILE FAIL");
			// if( fr != null ){
			// 		 fr.close();
			// 	}
			// if( fw != null ){
			// 		 fw.close();
			// 	}
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
	private static Integer findRandomOpenPortOnAllLocalInterfaces() throws IOException {
    try (
        ServerSocket socket = new ServerSocket(0);
    ) {
      return socket.getLocalPort();
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

}//fin de clase
