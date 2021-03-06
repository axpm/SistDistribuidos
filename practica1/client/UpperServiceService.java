
package client;

import java.net.MalformedURLException;
import java.net.URL;
import javax.xml.namespace.QName;
import javax.xml.ws.Service;
import javax.xml.ws.WebEndpoint;
import javax.xml.ws.WebServiceClient;
import javax.xml.ws.WebServiceException;
import javax.xml.ws.WebServiceFeature;


/**
 * This class was generated by the JAX-WS RI.
 * JAX-WS RI 2.2.9-b130926.1035
 * Generated source version: 2.2
 * 
 */
@WebServiceClient(name = "UpperServiceService", targetNamespace = "http://upper/", wsdlLocation = "file:/home/apmpm/Uni/Tercero/Distribuidos/SistDistribuidos/practica1/UpperService.wsdl")
public class UpperServiceService
    extends Service
{

    private final static URL UPPERSERVICESERVICE_WSDL_LOCATION;
    private final static WebServiceException UPPERSERVICESERVICE_EXCEPTION;
    private final static QName UPPERSERVICESERVICE_QNAME = new QName("http://upper/", "UpperServiceService");

    static {
        URL url = null;
        WebServiceException e = null;
        try {
            url = new URL("file:/home/apmpm/Uni/Tercero/Distribuidos/SistDistribuidos/practica1/UpperService.wsdl");
        } catch (MalformedURLException ex) {
            e = new WebServiceException(ex);
        }
        UPPERSERVICESERVICE_WSDL_LOCATION = url;
        UPPERSERVICESERVICE_EXCEPTION = e;
    }

    public UpperServiceService() {
        super(__getWsdlLocation(), UPPERSERVICESERVICE_QNAME);
    }

    public UpperServiceService(WebServiceFeature... features) {
        super(__getWsdlLocation(), UPPERSERVICESERVICE_QNAME, features);
    }

    public UpperServiceService(URL wsdlLocation) {
        super(wsdlLocation, UPPERSERVICESERVICE_QNAME);
    }

    public UpperServiceService(URL wsdlLocation, WebServiceFeature... features) {
        super(wsdlLocation, UPPERSERVICESERVICE_QNAME, features);
    }

    public UpperServiceService(URL wsdlLocation, QName serviceName) {
        super(wsdlLocation, serviceName);
    }

    public UpperServiceService(URL wsdlLocation, QName serviceName, WebServiceFeature... features) {
        super(wsdlLocation, serviceName, features);
    }

    /**
     * 
     * @return
     *     returns UpperService
     */
    @WebEndpoint(name = "UpperServicePort")
    public UpperService getUpperServicePort() {
        return super.getPort(new QName("http://upper/", "UpperServicePort"), UpperService.class);
    }

    /**
     * 
     * @param features
     *     A list of {@link javax.xml.ws.WebServiceFeature} to configure on the proxy.  Supported features not in the <code>features</code> parameter will have their default values.
     * @return
     *     returns UpperService
     */
    @WebEndpoint(name = "UpperServicePort")
    public UpperService getUpperServicePort(WebServiceFeature... features) {
        return super.getPort(new QName("http://upper/", "UpperServicePort"), UpperService.class, features);
    }

    private static URL __getWsdlLocation() {
        if (UPPERSERVICESERVICE_EXCEPTION!= null) {
            throw UPPERSERVICESERVICE_EXCEPTION;
        }
        return UPPERSERVICESERVICE_WSDL_LOCATION;
    }

}
