#ifndef INCLUDE_TABLECOLUMN_H_
#define INCLUDE_TABLECOLUMN_H_

#include "NullableField.h"

namespace ORMPlusPlus{

class TableColumn{
private:
	std::string name;
	DataType type;
	bool isPrimaryKey = false;
public:
    TableColumn(){};
    TableColumn(DataType type, std::string name){
	this->type = type;
	this->name = name;
    }
    
    void setAsPrimaryKey(){
	isPrimaryKey = true;
    }
    
    const std::string getName(){
	return name;
    }

};

}

#endif /* INCLUDE_TABLECOLUMN_H_ */
