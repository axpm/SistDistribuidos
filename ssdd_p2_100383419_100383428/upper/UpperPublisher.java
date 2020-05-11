package upper;

import javax.xml.ws.Endpoint;

//Clase para publicar el servicio
public class UpperPublisher {
  public static void main(String[ ] args) {
    final String url = "http://localhost:8888/rs";
    System.out.println("Publishing UpperService at endpoint " + url);
    Endpoint.publish(url, new UpperService());
  }
}
