#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <iomanip>
#include <vector>
#include <limits.h>
#include <unordered_map>

using namespace std;

int get_month_index( string name )
{
    map<string, int> months
    {
        { "Jan", 1 },
        { "Feb", 2 },
        { "Mar", 3 },
        { "Apr", 4 },
        { "May", 5 },
        { "Jun", 6 },
        { "Jul", 7 },
        { "Aug", 8 },
        { "Sep", 9 },
        { "Oct", 10 },
        { "Nov", 11 },
        { "Dec", 12 }
    };

    const auto iter = months.find( name );

    if( iter != months.cend() )
        return iter->second;
    return -1;
}

int main(int argc, char *argv[]){
	int interval=1;
	unsigned long int inceput=0;
	unsigned long int sfarsit=ULONG_MAX;
	ifstream f(argv[1]);
	string aux(argv[1]);
	aux.replace(aux.length()-3,3,"ref");
	ofstream of(aux);
	if(argc>2){
		string interv(argv[2]);
		if(interv.compare("--interval")==0)
			interval=atoi(argv[3]);
		if(argc > 5){
			interv.assign(argv[4]);
			if(interv.compare("--start")==0)
				inceput=atoi(string(argv[5]).substr(0,4).c_str())*60*24*30*30
						+atoi(string(argv[5]).substr(5,2).c_str())*60*24*30
						+atoi(string(argv[5]).substr(8,2).c_str())*60*24
						+atoi(string(argv[5]).substr(11,2).c_str())*60
						+atoi(string(argv[5]).substr(14,2).c_str());
			if(interv.compare("--end")==0)
				sfarsit=atoi(string(argv[5]).substr(0,4).c_str())*60*24*30*30
						+atoi(string(argv[5]).substr(5,2).c_str())*60*24*30
						+atoi(string(argv[5]).substr(8,2).c_str())*60*24
						+atoi(string(argv[5]).substr(11,2).c_str())*60
						+atoi(string(argv[5]).substr(14,2).c_str());
			if(argc > 7){
			
				interv.assign(argv[6]);
				if(interv.compare("--end")==0)
					sfarsit=atoi(string(argv[7]).substr(0,4).c_str())*60*24*30*30
							+atoi(string(argv[7]).substr(5,2).c_str())*60*24*30
							+atoi(string(argv[7]).substr(8,2).c_str())*60*24
							+atoi(string(argv[7]).substr(11,2).c_str())*60
							+atoi(string(argv[7]).substr(14,2).c_str());
			}
		}
	}
	map<string, double *> cale;//HASHTABLE CU ADRESA, ERROR CODES
	map<string, double *>::iterator it;
	unsigned long int ora_inceput;
	vector<map<string, string>::iterator> vec;// AJUTOR SORTARE HASHTABLE DUPA VALUE
	map<string, string> lista;// HASHTABLE CU ADRESA, DATA
	map<string, string>::iterator itl;
		while(!f.eof()){
			string x;
			getline(f,x);
			if(x.size()>0){
				string adresa(x.substr(x.find("GET ")+4,x.find(".html")-x.find("GET ")+1));
				string data(x.substr(x.find("[")+1,17));
				unsigned long int ora_curenta=atoi(data.substr(7,4).c_str())*60*24*30*30
												+get_month_index(data.substr(3,3))*30*1440
												+atoi(x.substr(x.find("[")+1,2).c_str())*1440
												+atoi(x.substr(x.find("[")+1,17).substr(12,2).c_str())*60
												+atoi(x.substr(x.find("[")+1,17).substr(15,2).c_str());
				if(inceput>ora_curenta) continue;
				//if(sfarsit<ora_curenta) break;
				//TODO: DE INCERCAT CU UNORDERED MAP
				for(itl=lista.begin();itl!=lista.end();itl++){
					ora_inceput=atoi((itl->second).substr(7,4).c_str())*60*24*30*30
								+get_month_index((itl->second).substr(3,3))*30*1440
								+atoi((itl->second).substr(0,2).c_str())*1440
								+atoi((itl->second).substr(12,2).c_str())*60
								+atoi((itl->second).substr(15,2).c_str());
					if(ora_curenta >= ora_inceput + interval)
						vec.push_back(itl);
				}
				
				if(!vec.empty())
				for(int i=0;i<vec.size()-1;i++)
					for(int j=i;j<vec.size();j++){
						unsigned long int a=atoi((vec[i]->second).substr(7,4).c_str())*60*24*30*30
											+get_month_index((vec[i]->second).substr(3,3))*30*1440
											+atoi((vec[i]->second).substr(0,2).c_str())*1440
											+atoi((vec[i]->second).substr(12,2).c_str())*60
											+atoi((vec[i]->second).substr(15,2).c_str());
						unsigned long int b=atoi((vec[j]->second).substr(7,4).c_str())*60*24*30*30
											+get_month_index((vec[j]->second).substr(3,3))*30*1440
											+atoi((vec[j]->second).substr(0,2).c_str())*1440
											+atoi((vec[j]->second).substr(12,2).c_str())*60
											+atoi((vec[j]->second).substr(15,2).c_str());
						if(a > b){
							map<string, string>::iterator aux=vec[i];
							vec[i]=vec[j];
							vec[j]=aux;
						}
						else
						if(a == b)
							if((vec[i]->first).compare(vec[j]->first)>0){
								map<string, string>::iterator aux=vec[i];
								vec[i]=vec[j];
								vec[j]=aux;
							}
					}
		
				for(int i=0;i<vec.size();i++){
					cout<<(vec[i]->second).substr(7,4)<<"-";
					if(get_month_index((vec[i]->second).substr(3,3))/10<1) cout<<"0";
					cout<<get_month_index((vec[i]->second).substr(3,3))<<"-"<<(vec[i]->second).substr(0,2);
					cout<<"T"<<(vec[i]->second).substr(12,5)<<" "<<interval<<" ";
					cout<<vec[i]->first<<" "<<fixed<<setprecision(2)
						<<cale.find(vec[i]->first)->second[0]*100/(cale.find(vec[i]->first)->second[0]+cale.find(vec[i]->first)->second[1])<<endl;
				}
				vec.clear();

				for(itl=lista.begin();itl!=lista.end();){
					ora_inceput=atoi((itl->second).substr(7,4).c_str())*60*24*30*30
								+get_month_index((itl->second).substr(3,3))*30*1440
								+atoi((itl->second).substr(0,2).c_str())*1440
								+atoi((itl->second).substr(12,2).c_str())*60
								+atoi((itl->second).substr(15,2).c_str());
					if(ora_curenta >= ora_inceput + interval){
						cale.erase(cale.find(itl->first));
						lista.erase(itl++);
					}
					else ++itl;
				}

				if(lista.find(adresa) == lista.end()){
					lista.insert(pair<string,string>(adresa,data));
					double *v=new double[2];
					v[0]=v[1]=0;
					cale.insert(pair<string,double *>(adresa,v));
					if(atoi(x.substr(x.find("HTTP/1.1")+10,3).c_str())/10/10==2) cale.find(adresa)->second[0]++;
							else cale.find(adresa)->second[1]++;
				}
				else{
					ora_inceput=atoi((lista.find(adresa)->second).substr(7,4).c_str())*60*24*30*30
								+get_month_index((lista.find(adresa)->second).substr(3,3))*30*1440
								+atoi((lista.find(adresa)->second).substr(0,2).c_str())*1440
								+atoi((lista.find(adresa)->second).substr(12,2).c_str())*60
								+atoi((lista.find(adresa)->second).substr(15,2).c_str());
					if(ora_curenta<ora_inceput+interval){
						if(atoi(x.substr(x.find("HTTP/1.1")+10,3).c_str())/10/10==2) cale.find(adresa)->second[0]++;
							else cale.find(adresa)->second[1]++;
					}
					else{
						cout<<(lista.find(adresa)->second).substr(7,4)<<"-";
						if(get_month_index((lista.find(adresa)->second).substr(3,3))/10<1) cout<<"0";
						cout<<get_month_index((lista.find(adresa)->second).substr(3,3))<<"-"<<(lista.find(adresa)->second).substr(0,2);
						cout<<"T"<<(lista.find(adresa)->second).substr(12,5)<<" "<<interval<<" ";
						cout<<adresa<<" "<<fixed<<setprecision(2)
							<<cale.find(adresa)->second[0]*100/(cale.find(adresa)->second[0]+cale.find(adresa)->second[1])<<endl;

						lista.erase(lista.find(adresa));
						cale.erase(cale.find(adresa));
					}
				}
		
			}
		}

		for(itl=lista.begin();itl!=lista.end();itl++)
			vec.push_back(itl);

		for(int i=0;i<vec.size()-1;i++)
			for(int j=i;j<vec.size();j++){
					unsigned long int a=atoi((vec[i]->second).substr(7,4).c_str())*60*24*30*30
										+get_month_index((vec[i]->second).substr(3,3))*30*1440
										+atoi((vec[i]->second).substr(0,2).c_str())*1440
										+atoi((vec[i]->second).substr(12,2).c_str())*60
										+atoi((vec[i]->second).substr(15,2).c_str());
					unsigned long int b=atoi((vec[j]->second).substr(7,4).c_str())*60*24*30*30
										+get_month_index((vec[j]->second).substr(3,3))*30*1440
										+atoi((vec[j]->second).substr(0,2).c_str())*1440
										+atoi((vec[j]->second).substr(12,2).c_str())*60
										+atoi((vec[j]->second).substr(15,2).c_str());
				if( a > b ){
					map<string, string>::iterator aux=vec[i];
					vec[i]=vec[j];
					vec[j]=aux;
				}
				else
				if(a == b)
					if((vec[i]->first).compare(vec[j]->first)>0){
						map<string, string>::iterator aux=vec[i];
						vec[i]=vec[j];
						vec[j]=aux;
					}
			}
			
		for(int i=0;i<vec.size();i++){
			cout<<(vec[i]->second).substr(7,4)<<"-";
			if(get_month_index((vec[i]->second).substr(3,3))/10<1) cout<<"0";
			cout<<get_month_index((vec[i]->second).substr(3,3))<<"-"<<(vec[i]->second).substr(0,2);
			cout<<"T"<<(vec[i]->second).substr(12,5)<<" "<<interval<<" ";
			cout<<vec[i]->first<<" "<<fixed<<setprecision(2)
				<<cale.find(vec[i]->first)->second[0]*100/(cale.find(vec[i]->first)->second[0]+cale.find(vec[i]->first)->second[1])<<endl;
		}
			
	return 0;
}
