#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include "analysis.h"
#include <vector>

namespace dh{

class generator{
	public:
		generator( analysis ans)
		{
			this->midcodes.resize(ans.midcodes.size());
			this->midcodes.assign(ans.midcodes.begin(), ans.midcodes.end());
			this->symTab = ::std::map< ::std::string, int>(ans.symTab);
		}

		void genAsmCode( const ::std::string& path);
	private:
		::std::vector<trival> midcodes;
		::std::map< ::std::string, int> symTab;
};
}
#endif
