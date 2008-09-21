#include <string>
#include <vector>

using namespace std;

class Template {
	private:
		vector<string> templates;
		string templateDir;

	public:
		Template();
		vector<string> listTemplates();
		string getTemplate(string templateName);		

};
