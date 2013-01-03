package rrdsolar;

public class Main {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {       

        MyDOMApp myDom = new MyDOMApp();
        myDom.parseXmlFile("dsctemp.xml");
        myDom.parseDocument();

    }

}
