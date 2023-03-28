// csvToWhatsappLink.cpp: This software reads a CSV file and generates an HTML containing the link to start a conversation with a WhatsApp contact.
// Author: Wesley Goncalves - wesley.goncalves88@gmail.com

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Contact
{
    std::string name;
    std::string phone;

    Contact()
    {
    }

    Contact(std::string _name, std::string _phone)
    {
        this->name = _name;
        this->phone = _phone;
    }
};

std::vector<std::string> getData(const std::string& rawData, char delimiter)
{
    std::vector<std::string> data;
    std::size_t pos = 0, lastPos = 0, counter = 0;
  
    for ( ; pos < rawData.length(); pos++)
    {
        if (rawData[pos] == delimiter)
        {
            data.push_back(rawData.substr(lastPos, counter));
            counter = 0;
            lastPos = pos+1;
        }
        else
        {
            counter++;
        }
    }
        
    return data;
}

int generateHTML(const std::vector<Contact>& contacts, const char* outputFile)
{
    std::string row;

    std::ofstream ofs;
    ofs.open(outputFile, std::ofstream::out);
    if (ofs.is_open())
    {
        row = "<html><body><h1>Contacts - WhatsApp Link</h1><ol>";

        ofs.write(row.c_str(), row.size());

        for (auto d : contacts)
        {
            row = "<li>" + d.name + " - <a href=\"https://api.whatsapp.com/send/?phone=" + d.phone + "\">whatsapp</a> </li>";
            ofs.write(row.c_str(), row.size());
        }

        row = "</ol></body></html>";
        ofs.write(row.c_str(), row.size());
        ofs.close();

        std::cout << "The HTML file was generated successfully!" << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "Error opening output file " << outputFile << std::endl;
        return 2;
    }
}

std::string formatNumber(std::string number)
{
    std::string result = "55";
    for (auto n : number)
    {
        if (n >= '0' && n <= '9')
        {
            result.push_back(n);
        }
    }
    
    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "ERROR: missing parameters" << std::endl
            << std::endl << "Expected usage: csvToWhatsappLink ${inputCSVFileLocation} ${outputHTMLFileLocation}" << std::endl
            << std::endl << "Example (on Windows): csvToWhatsappLink C:\\\\contacts.csv C:\\\\contacts.html";

        return 1;
    }

    constexpr uint8_t INPUT_CSV_FILE_ARGV_INDEX = 1;
    constexpr uint8_t OUTPUT_HTML_FILE_ARGV_INDEX = 2;

    std::ifstream ifs;
    constexpr char delimiter = ',';
    std::vector<Contact> contacts;

    ifs.open(argv[INPUT_CSV_FILE_ARGV_INDEX], std::ifstream::in);
    if (ifs.is_open())
    {
        std::cout << "Reading contacts..." << std::endl;
        std::string line;
        std::getline(ifs, line); // ignore first line
        while (std::getline(ifs, line))
        {
            std::vector<std::string> data = getData(line, delimiter);
            std::string phone = formatNumber(data[1]);
            Contact c(data[0], phone);
            contacts.push_back(c);
        }

        std::cout << "Number of contacts read: " << contacts.size() << std::endl;
    }
    else
    {
        std::cerr << "Error opening input file " << argv[INPUT_CSV_FILE_ARGV_INDEX] << std::endl;
        return 2;
    }

    return generateHTML(contacts, argv[OUTPUT_HTML_FILE_ARGV_INDEX]);
}

