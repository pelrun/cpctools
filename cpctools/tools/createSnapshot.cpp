/**
* Permet d'afficher le contenu d'un fichier snapshot CPC
* @author Thierry JOUIN
* @version 1.1
* @date 31/10/2001
*/

#include "CError.h"
#include "CCPCSnapshot.h"
#include "COptionParser.h"

#include <vector>
#include <cstdlib>

using namespace std;

struct SOption appliOption[]=
{
	{'i',"inSnapshot",0,1,1,"Load <$1> snapshot file"},
	{'l',"loadFileData",0,0,2,"Load <$1> file data at <$2> address in snapshot memory (or use AMSDOS header load address if <$2> is negative)"},
	{'p',"putData",0,0,2,"Put <$2> byte at <$1> address in snapshot memory"},
	{'s',"setToken",0,0,2,"Set snapshot token <$1> to value <$2>\n\t\t"
		"Use <$1>:<val> to set array value\n\t\t"
		"ex '-s CRTC_REG:6 20' : Set CRTC register 6 to 20"},
	{'x',"clearMemory",0,1,0,"Clear snapshot memory"},
	{'r',"romDeconnect",0,1,0,"Disconnect lower and upper rom"},
	{'e',"enableInterrupt",0,1,0,"Enable interrupt"},
	{'d',"disableInterrupt",0,1,0,"Disable interrupt"},
	{'c',"configFile",0,1,1,"Load a config file with createSnapshot option"},
	{'t',"tokenList",0,1,0,"Display setable snapshot token ID"},
	{'o',"output",0,0,3,"Output <$3> bytes of data from address <$2> to file <$1>"},
	{'f',"fillData",0,0,3,"Fill snapshot from <$1> over <$2> bytes, with <$3> datas"},
	{'g',"fillText",0,0,3,"Fill snapshot from <$1> over <$2> bytes, with <$3> text"},
	{'j',"loadIniFile",0,1,1,"Load <$1> init file"},
	{'k',"saveIniFile",0,1,1,"Save <$1> init file"},
	{0,NULL,0,0,0,NULL}
};

static const string authorName = "Ramlaid & cpcsdk team";
static const string authorMail = "cpcsdk@googlegroups.com";
static const string appliName = "createSnapshot";
static const string appliUsageShort = "<filename.sna>";
static const string appliUsageLong = "<filename.sna>\n"
	"Create a Amstrad CPC snapshot <filename.sna>\n"
	"Default snapshot : 6128, CRTC 0, rom deconnected, no interrupt, IM 1, "
    "SP = 0xc000, CRTC standard value";

unsigned char getValue(const string &strVal)
{
	if (strVal.size() == 3 && strVal[0] == '\'' && strVal[2] == '\'')
	{
		return strVal[1];
	}

	int val;
	val = strtol(strVal.c_str(),NULL,0);
	TOOLS_ASSERTMSG(errno != ERANGE, "Wrong value " << strVal);
	return (unsigned char)val;
}

int main(int argc, char** argv)
{
	try
	{
		int i;

		COptionParser optParser(appliOption, appliName, appliUsageShort, appliUsageLong, authorName, authorMail);
		
		optParser.PrintHeader(cout);

		if (!optParser.ParseCommandLine(&argc, &argv, 1, true))
		{
			optParser.PrintError(cout);
			optParser.PrintUsage(cout);
			return -1;
		}

		string filenameOut(argv[1]);
		CCPCSnapshot snapshot;

		// Looking for token help display
		if (optParser.FindOption('t'))
		{
			CCPCSnapshot::displayTokenHelp(cout);
			return 0;
		}

		// Looking for a -i arg
		if (optParser.FindOption('i'))
		{
			string includeFile;
			includeFile = optParser.GetStringOption('i', "");
			TOOLS_ASSERTMSG(!includeFile.empty(), "No include file defined");

			cout << "Load input snapshot " << includeFile << endl;
			snapshot.loadFromFile(includeFile);
		}
		
		// Looking for a -j arg
		if (optParser.FindOption('j'))
		{
			string iniFile = optParser.GetStringOption('j', "");
			TOOLS_ASSERTMSG(!iniFile.empty(), "No ini file defined");

			cout << "Load ini snapshot " << iniFile << endl;
			snapshot.loadSnapshotIni(iniFile);
		}

		if (optParser.FindOption('x'))
		{
			cout << "Clear memory" << endl;
			snapshot.clearMemory();
		}

		i=0;
		while (optParser.GetOptionI(i) != 0)
		{
			switch(optParser.GetOptionI(i))
			{
			case 'l':
				{
					string filename = optParser.GetStringOptionI(i, 0);
					int adr = optParser.GetIntOptionI(i, 1);

					cout << "Load data " << filename;
					if(adr > 0)   
						cout << " at address #" << hex << adr << dec << endl;
					else
						cout << " using AMSDOS header information" << endl;
					snapshot.loadDataFromFile(filename,adr);
					break;
				}
			case 'p':
				{
					int adr = optParser.GetIntOptionI(i, 0);
					int val = optParser.GetIntOptionI(i, 1);

					cout << "Putting data #" << hex << val << " at address #" << adr << dec << endl;
					snapshot.putData(adr,val);
					break;
				}
			case 's':
				{
					string token = optParser.GetStringOptionI(i, 0);
					int val = optParser.GetIntOptionI(i, 1);
					
					cout << "Setting token " << token << " to " << val << endl;
					snapshot.setTokenValue(const_cast<char*>(token.c_str()), val);
					break;
				}
			case 'r':
				{
					cout << "Rom deconnection" << endl;
					snapshot.romDeconnect();
					break;
				}
			case 'd':
				{
					cout << "Interrupt disable" << endl;
					snapshot.disableInterrupt();
					break;
				}
			case 'e':
				{
					cout << "Interrupt enable" << endl;
					snapshot.enableInterrupt();
					break;
				}
			case 'o':
				{
					string filename = optParser.GetStringOptionI(i, 0);
					int adr = optParser.GetIntOptionI(i, 1);
					int size = optParser.GetIntOptionI(i, 2);

					snapshot.saveDataToFile(filename,adr,size);
					cout << "Save #" << hex << size << "bytes of data from address #" << adr << " to file " << filename << dec << endl;
					break;
				}
			case 'f':
				{
					int adr = optParser.GetIntOptionI(i, 0);
					string datas = optParser.GetStringOptionI(i, 2);
					int size = optParser.GetIntOptionI(i, 1);

					vector<unsigned char> dataArray;
					string strVal;
					size_t start = 0;
					size_t pos = datas.find(',', start);
					while (pos != string::npos)
					{
						strVal = datas.substr(start, pos - start);
						dataArray.push_back((unsigned char)getValue(strVal));
						start = pos+1;
						pos = datas.find(',', start);
					}
					if (pos-start > 1)
					{
						strVal = datas.substr(start, pos - start);
						dataArray.push_back((unsigned char)getValue(strVal));
					}

					snapshot.fillData(adr, size, dataArray);

					break;
				}
			case 'g':
				{
					int adr = optParser.GetIntOptionI(i, 0);
					string text = optParser.GetStringOptionI(i, 2);
					int size = optParser.GetIntOptionI(i, 1);

					vector<unsigned char> dataArray;
					dataArray.resize(text.size());
					copy(text.begin(), text.end(), dataArray.begin());

					snapshot.fillData(adr, size, dataArray);

					break;
				}
			case 'c':
			case 'x':
			case 'i':
			case 'j':
			case 'k':
				{
					// Already done !
					break;
				}
			default :
				{
					cerr << "Unknow option ! " << optParser.GetOptionI(i) << endl;
					optParser.PrintUsage(cout);
					return -1;
				}
			}
			i++;
		}
		
		cout << "Saving " << filenameOut << endl;
		snapshot.saveToFile(filenameOut);

		// Looking for a -k arg
		if (optParser.FindOption('k'))
		{
			string iniFile = optParser.GetStringOption('k', "");
			TOOLS_ASSERTMSG(!iniFile.empty(), "No ini file defined");

			cout << "Save ini snapshot " << iniFile << endl;
			snapshot.saveSnapshotIni(iniFile);
		}

	}
	catch(tools::CException &e)
	{
		cerr << e << endl;
		return -1;
	}
	return 0;
}
