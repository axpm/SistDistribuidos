import java.lang.* ;
import java.io.* ;
import java.net.* ;
import java.util.* ;


public class cliente{
   public static void main ( String [] args){

      if (args.length != 2){
         System.out.println("Uso: cliente <host> <puerto>");
         System.exit(0);
      }

     try{
        // Crear las conexiones
	       // incluir el código aqui
        String host = args[0];
        int socket = Integer.parseInt(args[1]);
        Socket sc = new Socket(host, socket);

        DataOutputStream out = new DataOutputStream(sc.getOutputStream()); //Enviar
        DataInputStream in = new DataInputStream(sc.getInputStream()); //Recibir

        InputStreamReader  is = new InputStreamReader(System.in); //Leemos por pantalla
        BufferedReader br = new BufferedReader(is); //Guardamos

        String mensaje;
        boolean hecho = false;


        while (!hecho) {
		      // completar el código para comunicar con el servidor
          System.out.println("Escriba algo");

          mensaje = br.readLine(); //Coge el mensaje del buffer del mensaje de pantalla

          out.writeBytes(mensaje); //Escribimos en la salida del cliente
          out.write('\0'); // inserta el código ASCII 0 al final

          byte[] ch = new byte[1];
          String mensajeServer = new String();
          do{
            ch[0] = in.readByte(); //Leemos la respuesta
            if (ch[0] != '\0'){
              String d = new String(ch);
              mensajeServer = mensajeServer + d; //Concatenamos
            }
          }while(ch[0] != '\0');

          if (mensajeServer.equals("exit")){ //Si recibe exit, terminamos
            hecho = true;
          }else{
            System.out.println(mensajeServer); //Imprimimos el mensaje
          }

        }
        sc.close();

        return;

      }
      catch (Exception e)
      {
         System.err.println("excepcion " + e.toString() );
         e.printStackTrace() ;
      }

   }
}
