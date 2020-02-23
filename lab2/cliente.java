import java.lang.* ;
import java.io.* ;
import java.net.* ;
import java.util.* ;


public class cliente{
   public static void main ( String [] args){

      if (args.length != 2)
      {
         System.out.println("Uso: cliente <host> <puerto>");
         System.exit(0);
      }

     try
     {
        // Crear las conexiones
	// incluir el código aqui



	InputStreamReader  is = new InputStreamReader(System.in);
	BufferedReader br = new BufferedReader(is);

        String mensaje;
        boolean hecho = false;

        while (!hecho) {
		// completar el código para comunicar con el servidor
                System.out.println("Escriba algo");

                 mensaje = br.readLine();

		 System.out.println(mensaje);

        }


      }
      catch (Exception e)
      {
         System.err.println("excepcion " + e.toString() );
         e.printStackTrace() ;
      }

   }
}
