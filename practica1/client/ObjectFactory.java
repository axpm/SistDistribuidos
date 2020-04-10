
package client;

import javax.xml.bind.JAXBElement;
import javax.xml.bind.annotation.XmlElementDecl;
import javax.xml.bind.annotation.XmlRegistry;
import javax.xml.namespace.QName;


/**
 * This object contains factory methods for each 
 * Java content interface and Java element interface 
 * generated in the client package. 
 * <p>An ObjectFactory allows you to programatically 
 * construct new instances of the Java representation 
 * for XML content. The Java representation of XML 
 * content can consist of schema derived interfaces 
 * and classes representing the binding of schema 
 * type definitions, element declarations and model 
 * groups.  Factory methods for each of these are 
 * provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

    private final static QName _StandardizeString_QNAME = new QName("http://upper/", "standardizeString");
    private final static QName _StandardizeStringResponse_QNAME = new QName("http://upper/", "standardizeStringResponse");

    /**
     * Create a new ObjectFactory that can be used to create new instances of schema derived classes for package: client
     * 
     */
    public ObjectFactory() {
    }

    /**
     * Create an instance of {@link StandardizeString }
     * 
     */
    public StandardizeString createStandardizeString() {
        return new StandardizeString();
    }

    /**
     * Create an instance of {@link StandardizeStringResponse }
     * 
     */
    public StandardizeStringResponse createStandardizeStringResponse() {
        return new StandardizeStringResponse();
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link StandardizeString }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://upper/", name = "standardizeString")
    public JAXBElement<StandardizeString> createStandardizeString(StandardizeString value) {
        return new JAXBElement<StandardizeString>(_StandardizeString_QNAME, StandardizeString.class, null, value);
    }

    /**
     * Create an instance of {@link JAXBElement }{@code <}{@link StandardizeStringResponse }{@code >}}
     * 
     */
    @XmlElementDecl(namespace = "http://upper/", name = "standardizeStringResponse")
    public JAXBElement<StandardizeStringResponse> createStandardizeStringResponse(StandardizeStringResponse value) {
        return new JAXBElement<StandardizeStringResponse>(_StandardizeStringResponse_QNAME, StandardizeStringResponse.class, null, value);
    }

}
