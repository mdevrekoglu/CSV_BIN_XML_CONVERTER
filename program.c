#include <stdio.h>
#include <libxml/xmlschemastypes.h>
#include <stdbool.h>
#include <string.h>

int validate(char *XMLFileName, char *XSDFileName);
int convertCSVtoBinary(char *CSVFileName, char *BinaryFileName);
int convertSTRtoBIN();

// Our project is reading an csv file and converting it to binary file then reading that binary file and
// Converting it to xml file and then validating it with xsd file. The language we are using is C.
int main()
{
    char *CSVFileName = "records.csv";
    char *BinaryFİleName = "binary.bin"; //!!!!!!!!!!
    char *XMLFileName = "records.xml"; // write your xml file here
    char *XSDFileName = "records.xsd"; // write your xsd file here

    // send copy of the string to the function
    char str[strlen(CSVFileName)];
    strcpy(str, CSVFileName);
    int a = convertSTRtoBIN(str);


    // convert str to char array




    //int BinaryFileState = convertCSVtoBinary(CSVFileName, BinaryFİleName);
    //printf("Binary State: %d\n", BinaryFileState);

    //int ValidateState = validate(XMLFileName, XSDFileName);
    //printf("Validate State: %d\n", ValidateState);
    
    return 0;  
}

int convertCSVtoBinary(char *CSVFileName, char *BinaryFileName){

    FILE* fpCSV = fopen(CSVFileName, "r");
    FILE* fpBIN = fopen(BinaryFileName, "w");

    if(!fpCSV){
        printf("An error accoured during opening %s\n", CSVFileName);
        return -1;
    }
    if(!fpBIN){
        printf("An error accoured during opening %s\n", BinaryFileName);
        return -1;
    }

    // Assuming every line has maximum 1000 characters
    char row[1000];
    char bin[1000];


    while(feof(fpCSV) != true){
        fgets(row, 1000, fpCSV);
        printf("Row: %s",row);

        char * token = strtok(row, ",");

        while(token != NULL){
            printf("%s ", token);
            token = strtok(NULL, ",");
        }
        printf("\n");
    }


    fclose(fpCSV);
    fclose(fpBIN);

    return 0;
}

int convertSTRtoBIN(char str[]){
    

    for (int i = 0; i < strlen(str); i++)
    {
        printf("%c", str[i]);
    }
    

    return 0;
}

int validate(char *XMLFileName, char *XSDFileName){
    xmlDocPtr doc;
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;    
    
    xmlLineNumbersDefault(1); //set line numbers, 0> no substitution, 1>substitution
    ctxt = xmlSchemaNewParserCtxt(XSDFileName); //create an xml schemas parse context
    schema = xmlSchemaParse(ctxt); //parse a schema definition resource and build an internal XML schema
    xmlSchemaFreeParserCtxt(ctxt); //free the resources associated to the schema parser context
    
    doc = xmlReadFile(XMLFileName, NULL, 0); //parse an XML file
    if (doc == NULL)
    {
        fprintf(stderr, "Could not parse %s\n", XMLFileName);
    }
    else
    {
        xmlSchemaValidCtxtPtr ctxt;  //structure xmlSchemaValidCtxt, not public by API
        int ret;
        
        ctxt = xmlSchemaNewValidCtxt(schema); //create an xml schemas validation context 
        ret = xmlSchemaValidateDoc(ctxt, doc); //validate a document tree in memory
        if (ret == 0) //validated
        {
            printf("%s validates\n", XMLFileName);
        }
        else if (ret > 0) //positive error code number
        {
            printf("%s fails to validate\n", XMLFileName);
        }
        else //internal or API error
        {
            printf("%s validation generated an internal error\n", XMLFileName);
        }
        xmlSchemaFreeValidCtxt(ctxt); //free the resources associated to the schema validation context
        xmlFreeDoc(doc);
    }
    // free the resource
    if(schema != NULL)
        xmlSchemaFree(schema); //deallocate a schema structure

    xmlSchemaCleanupTypes(); //cleanup the default xml schemas types library
    xmlCleanupParser(); //cleans memory allocated by the library itself 
    xmlMemoryDump(); //memory dump
    return(0);
}

