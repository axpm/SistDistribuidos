package upper;

import javax.jws.WebService;
import javax.jws.WebMethod;
import java.util.Random;

@WebService
  public class UpperService{

    @WebMethod //Es un servicio Web para --> client
    public String standardizeString(String input) { //devuelve el String input, pero en mayúsculas
      return input.toUpperCase();
    }
    
}
