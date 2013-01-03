package rrdsolar;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class MyDOMApp {

    Document dom;

    public void parseXmlFile(String filename){
		//get the factory
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();

		try {

			//Using factory get an instance of document builder
			DocumentBuilder db = dbf.newDocumentBuilder();

			//parse using builder to get DOM representation of the XML file
			dom = db.parse(filename);


		}catch(ParserConfigurationException pce) {
			pce.printStackTrace();
		}catch(SAXException se) {
			se.printStackTrace();
		}catch(IOException ioe) {
			ioe.printStackTrace();
		}
	}

    public void parseDocument() {
        Element docEle = dom.getDocumentElement();
        ArrayList<String> comment = new ArrayList<String>();

        //get the 2nd database element
        NodeList nl = docEle.getElementsByTagName("database");
        Element database = (Element) nl.item(1);

        //get comments
        NodeList children = database.getChildNodes();
        for (int i=0; i< children.getLength(); i++) {
            if (children.item(i).getNodeType() == Node.COMMENT_NODE) {
                String[] tokens = children.item(i).getTextContent().split("[ ]+");
                comment.add(tokens[1]);
            }
        }

        //open output file for writing
        try {
            FileWriter fstream = new FileWriter("dsctemp.csv");
            BufferedWriter out = new BufferedWriter(fstream);

            //get values
            NodeList rows = database.getElementsByTagName("row");
            for (int i = 0; i < rows.getLength(); i++) {
                NodeList values = rows.item(i).getChildNodes();
                Element value = (Element) values.item(1);
                out.write(comment.get(i) + "\t");
                if (value.getTextContent().equals("NaN")) {
                    out.write("0.0000000000e+00\n");
                } else {
                    out.write(value.getTextContent() + "\n");
                }
            }
            out.close();
        } catch (Exception e) {
            System.err.print("Error:" + e.getMessage());
        }
    }

}
