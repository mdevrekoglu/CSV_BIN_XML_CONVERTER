#include <stdbool.h> //boolean type
#include <string.h> //string operations
#include <stdio.h> //standart buffered input/output
#include <string.h> //string operations
#include <stdlib.h> //standart library definitions
#include <math.h> // math operations
#include <libxml/xmlschemastypes.h> //xml schema types

void convertCSVtoBIN(char *CSVFileName, char *BinaryFileName);
void converBINtoXML(char *BinaryFileName, char *XMLFileName);
void validate(char *XMLFileName, char *XSDFileName);

typedef struct Person
{
    char name[21];
    char surname[31];
    char gender;
    char occupancy[31];
    char level_of_education[4];
    char email[31];
    char bank_account_number[13];
    char IBAN[28];
    char account_type[14];
    char currency_unit[4];
    unsigned int total_balance_available;
    char available_for_loan[5];
}Person;


int main(int argc, char *argv[])
{
    if(argc != 4){ // Checking whether given arguments are correct
        printf("Wrong number of arguments\n");
        printf("Usage: ./program <input_file> <output_file> <type>\n");
        printf("Type: 1 - CSV to BIN\n");
        printf("Type: 2 - BIN to XML\n");
        printf("Type: 3 - XML XSD verification\n");
        exit(-1);
    }

    int type = atoi(argv[3]); // Converting last argument to integer
    if(type == 1){ // If type is 1 then convert CSV to BIN
        convertCSVtoBIN(argv[1], argv[2]);
    }
    else if(type == 2){ // If type is 2 then convert BIN to XML
        converBINtoXML(argv[1], argv[2]);
    }
    else if(type == 3){ // If type is 3 then validate XML with XSD
        validate(argv[1], argv[2]);
    }
    else{ // If type is not 1, 2 or 3 then print error message
        printf("Wrong type\n");
        printf("Type: 1 - CSV to BIN\n");
        printf("Type: 2 - BIN to XML\n");
        printf("Type: 3 - XML XSD verification\n");
        exit(-1);
    }

    return 0; 
}

// Convert CSV to BIN
void convertCSVtoBIN(char *CSVFileName, char *BINFileName){
{
    FILE *CSVFileName = fopen("records.csv", "r");
    FILE *BINFileName = fopen("hadibabana.dat", "wb");
    if (!BINFileName) {
        printf("Failed to create binary file.\n");
        fclose(CSVFileName);
        exit(1);
    }

    char line[sizeof(Person)];
    char temp[sizeof(Person)]; // temporary for modified line
     
    Person person;
    
    fgets(line, sizeof(Person), CSVFileName);
    while (fgets(line, sizeof(Person), CSVFileName)) {
        int len = strlen(line);

        for (int i = 0; i < len-1; i++) {
            if (line[i] == ',' && line[i+1] == ',') {
                // insert "NULL" between the two commas
                
                memcpy(temp, line, i+1); // copy the part of the line up to the first comma
                strcpy(temp+i+1, " "); // insert "NULL" after the first comma
                strcpy(temp+i+2, line+i+1); // copy the part of the line after the second comma
                strcpy(line, temp); // copy the modified line back to the original buffer
                len += 1; // update the length of the line
            }
        }

          sscanf(line, "%20[^,],%30[^,],%c,%30[^,],%3[^,],%30[^,],%12[^,],%27[^,],%13[^,],%3[^,],%u,%4[^,]",
               person.name, person.surname, &person.gender, person.occupancy, person.level_of_education,
               person.email, person.bank_account_number, person.IBAN, person.account_type,
               person.currency_unit, &person.total_balance_available, person.available_for_loan);

        fwrite(&person, sizeof(Person), 1, BINFileName);
    }

    fclose(CSVFileName);
    fclose(BINFileName);
    
}
}

// Convert BIN to XML
void converBINtoXML(char *BINFileName, char *XMLFileName){

    // Open the given file in binart reading mode
    FILE *fpBIN = fopen(BINFileName, "rb");

    // Check if the file is opened
    if(!fpBIN){
        printf("An error accoured during opening %s\n", BINFileName);
        exit(-1);
    }

    // To calculate number of person go end of the file and divide by size of Person
    // It is not necessary but it is good to know
    fseek(fpBIN, 0, SEEK_END);
    int numberOfPerson = (ftell(fpBIN)/sizeof(Person));
    printf("\nNumber of person: %d\n\n", numberOfPerson);

    // Go back to the beginning of the file
    fseek(fpBIN, 0, SEEK_SET);

    // Create XML document and root node
    xmlDocPtr doc = NULL;
    xmlNodePtr rootNode = NULL;
    
    // Create a new XML document and a new root node
    doc = xmlNewDoc(BAD_CAST "1.0");
    rootNode = xmlNewNode(NULL, BAD_CAST "records");
    // Set the root node
    xmlDocSetRootElement(doc, rootNode);

    // Read from file and write to XML
    for(int i = 1; i <= numberOfPerson; i++){
        // Create a temporary structure
        Person person;
        // Clear the structure, we don't want garbage values
        memset(&person, 0, sizeof(Person));
        // Read from file in binary format and store it in person
        fread(&person, sizeof(Person), 1, fpBIN);

        // Converting variables
        char intToStr[11]; // 10 digits + '\0'
        char charToStr[2]; // 1 digit + '\0'
        intToStr[10] = '\0';
        charToStr[1] = '\0';
        
        // Create a new row node and set its attribute
        xmlNodePtr tempNode = xmlNewChild(rootNode, NULL, BAD_CAST "row", NULL);
        sprintf(intToStr, "%d", i);
        xmlNewProp(tempNode, BAD_CAST "id", BAD_CAST intToStr);

        // Create a subnode for row node
        xmlNodePtr customerInfo = xmlNewChild(tempNode, NULL, BAD_CAST "customer_info", NULL);

        // Create subnodes for customerInfo subnode and set their values
        xmlNewChild(customerInfo, NULL, BAD_CAST "name", BAD_CAST person.name);
        xmlNewChild(customerInfo, NULL, BAD_CAST "surname", BAD_CAST person.surname);
        charToStr[0] = person.gender;
        xmlNewChild(customerInfo, NULL, BAD_CAST "gender", charToStr);
        xmlNewChild(customerInfo, NULL, BAD_CAST "occupancy", BAD_CAST person.occupancy);
        xmlNewChild(customerInfo, NULL, BAD_CAST "level_of_education", BAD_CAST person.level_of_education);
        xmlNewChild(customerInfo, NULL, BAD_CAST "email", BAD_CAST person.email);

        // Create a subnode for row node
        xmlNodePtr bankAccountInfo = xmlNewChild(tempNode, NULL, BAD_CAST "bank_account_info", NULL);

        // Create subnodes for bankAccountInfo subnode and set their values
        xmlNewChild(bankAccountInfo, NULL, BAD_CAST "bank_account_number", BAD_CAST person.bank_account_number);
        xmlNewChild(bankAccountInfo, NULL, BAD_CAST "IBAN", BAD_CAST person.IBAN);
        xmlNewChild(bankAccountInfo, NULL, BAD_CAST "account_type", BAD_CAST person.account_type);
        xmlNewChild(bankAccountInfo, NULL, BAD_CAST "available_for_loan", BAD_CAST person.available_for_loan);
        
        // This node has an attribute so we need to create it separately
        sprintf(intToStr, "%d", person.total_balance_available);
        xmlNodePtr a = xmlNewChild(bankAccountInfo, NULL, BAD_CAST "total_balance_available", BAD_CAST intToStr);
        // Set the first attribute
        xmlNewProp(a, BAD_CAST "currency_unit", BAD_CAST person.currency_unit);
        
        // Unsigned int to binary string (4 bytes)
        unsigned int littleEnd_Version = person.total_balance_available;  
        // Little Endian to Big Endian (4 bytes) conversion part   
		char binarySTR[33]; // 32 digits + '\0'
		binarySTR[32] = '\0';
		int m = 31, n; // Variables to traverse the string
		while(littleEnd_Version != 0){ // Convert to binary
			if(littleEnd_Version % 2 == 0){
				binarySTR[m] = '0';
			}
			else{
				binarySTR[m] = '1';
			}

			littleEnd_Version /= 2;
			m--;
		}
		while(m >= 0){ // Fill the rest of the string with 0s to remove garbage values
			binarySTR[m] = '0';
			m--;
		}
        // For debugging
        //printf("Little End Str: %s\n", binarySTR);

        m = 0, n = 31; //Reset variables
        while(m < n){ // Reverse the string to get Big Endian
            char temp = binarySTR[m];
            binarySTR[m] = binarySTR[n];
            binarySTR[n] = temp;

            m++;
            n--;
        }
        // For debugging
        //printf("BigEnd Str: %s\n", binarySTR);

        // Binary string to unsigned int (4 bytes)
        unsigned int bigEnd_Version = 0;
        m = 0, n = 31; // Reset variables
        while(m <= n){ // Convert to unsigned int
            if(binarySTR[m] == '1'){
                bigEnd_Version += (int)pow((double) 2, (double) (31 - m));
            }
            m++;
        }
        // For debugging
        //printf("BigEnd Int: %u\n", bigEnd_Version);

        sprintf(intToStr, "%u", bigEnd_Version);
        xmlNewProp(a, BAD_CAST "bigEnd_Version", BAD_CAST intToStr);
    }

    // Save the XML file
    xmlSaveFormatFileEnc(XMLFileName, doc, "UTF-8", 1);
    // Print the XML file to the console (for debugging)
    //xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
    xmlFreeDoc(doc);

    xmlCleanupParser();
    fclose(fpBIN);
}

void validate(char *XMLFileName, char *XSDFileName){
    
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
}
