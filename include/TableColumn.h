#ifndef INCLUDE_TABLECOLUMN_H_
#define INCLUDE_TABLECOLUMN_H_

namespace ORMPlusPlus{

class TableColumn{
private:
	std::string name;
//	DataType type;
	bool isPrimaryKey = false;
public:
	void setAsPrimaryKey(){
		isPrimaryKey = true;
	}
//	void setType(DataType type){
//		this->type = type;
//	}
};

}

#endif /* INCLUDE_TABLECOLUMN_H_ */
