#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>

class OperationTree;

using TreePtr = std::unique_ptr<OperationTree>;

enum OperationType { Plus, Minus, Multiply, Divide, Number, Variable, BadType };

class OperationTree
{
private:
	OperationType type;
	int_least32_t number;
	TreePtr leftOperand;
	TreePtr rightOperand;
public:
	OperationTree()
	{
		type = Variable;
	};
	OperationTree(int_least32_t number)
	{
		type = Number;
		this->number = number;

	};
	OperationTree(std::unique_ptr<OperationTree> leftOperand, std::unique_ptr<OperationTree> rightOperand, OperationType type)
	{
		this->type = type;
		this->leftOperand = std::move(leftOperand);
		this->rightOperand = std::move(rightOperand);
	};

	OperationType GetType() {
		return type;
	}
	int_least32_t GetNumber() {
		return number;
	}


	void printTree()
	{
		
		switch (type) {
		case Plus:
			std::cout << "("; (*leftOperand).printTree(); std::cout << ")+("; (*rightOperand).printTree(); std::cout << ")";
			break;
		case Minus:
			std::cout << "("; (*leftOperand).printTree(); std::cout << ")-("; (*rightOperand).printTree(); std::cout << ")";
			break;
		case Multiply:
			std::cout << "("; (*leftOperand).printTree(); std::cout << ")*("; (*rightOperand).printTree(); std::cout << ")";
			break;
		case Divide:
			std::cout << "("; (*leftOperand).printTree(); std::cout << ")/("; (*rightOperand).printTree(); std::cout << ")";
			break;
		case Number:
			std::cout << number;
			break;
		case Variable:
			std::cout << "x";
			break;
		default: return;
		}
	}

	int_least32_t Evaluate(int_least32_t variableValue)
	{
		switch (type) {
		case Plus: 
			return (*leftOperand).Evaluate(variableValue) + (*rightOperand).Evaluate(variableValue);
		case Minus:
			return (*leftOperand).Evaluate(variableValue) - (*rightOperand).Evaluate(variableValue);
		case Multiply:
			return (*leftOperand).Evaluate(variableValue) * (*rightOperand).Evaluate(variableValue);
		case Divide:
			return (*leftOperand).Evaluate(variableValue) / (*rightOperand).Evaluate(variableValue);
		case Number:
			return number;
		case Variable:
			return variableValue;
		default: return 0;
		}
	}

};

struct element {
	int_least32_t x;
	OperationType type;
};

class InputParser {

private:
	std::ifstream is;
	char c;
public:
	InputParser(char * fileName) : is(std::ifstream(fileName))
	{
		is.get(c);

	}
	~InputParser() {
		is.close();
	}

	element ParseNext()
	{
		element el;
		bool isNumber = false;
		el.x = 0;
		while (is.good() && std::isspace(c))
		{
			is.get(c);			
		}
		while (is.good() && std::isdigit(c))
		{
			isNumber = true;
			el.x = el.x * 10 + (c - '0');
			is.get(c);
		}
		if (!is.good()) el.type = BadType;
		else if (!isNumber) {
			switch (c) {
			case 'x': el.type = Variable;
				break;
			case '+': el.type = Plus;
				break;
			case '-': el.type = Minus;
				break;
			case '*': el.type = Multiply;
				break;
			case '/': el.type = Divide;
				break;
			default: 
				throw 2;
			}
			is.get(c);
		}
		else el.type = Number;
		
		return el;

	}
};


struct MinMax {
	int_least32_t min;
	int_least32_t max;
};

MinMax FindMinMax(OperationTree & ot, int_least32_t start, int_least32_t end) {
	MinMax minmax;
	minmax.min = INT_LEAST32_MAX;
	minmax.max = INT_LEAST32_MIN;
	for (int_least32_t i = start;i <= end;++i) {
		int_least32_t val= ot.Evaluate(i);
		if (val > minmax.max) { minmax.max = val; }
		if (val < minmax.min) { minmax.min = val; }
	}
	return minmax;
 }


int main(int argc, char * argv[]) {
	if (argc != 4) {
		std::cout << "Wrong number of arguments" << std::endl;
		return 1;
	}
	int_least32_t start = strtol(argv[2], NULL, 10);
	int_least32_t end = strtol(argv[3], NULL, 10);


	std::vector<TreePtr> stack;
	try {
		InputParser in(argv[1]);


		element el = in.ParseNext();
		while (el.type != BadType) {
			std::unique_ptr<OperationTree> newTree;
			switch (el.type) {
			case Number:  newTree = std::make_unique<OperationTree>(el.x);
				break;
			case Variable: newTree = std::make_unique<OperationTree>();
				break;
			case Plus:
			case Minus:
			case Multiply:
			case Divide:
				if (stack.size() < 2) { std::cout << "Bad input" << std::endl; return 1; }
				TreePtr right = std::move(stack.back());
				stack.pop_back();
				TreePtr left = std::move(stack.back());
				stack.pop_back();
				if ((*right).GetType() == Number && (*left).GetType() == Number)
				{
					switch (el.type)
					{
					case Plus:
						newTree = std::make_unique<OperationTree>((*left).GetNumber() + (*right).GetNumber());
						break;
					case Minus:
						newTree = std::make_unique<OperationTree>((*left).GetNumber() - (*right).GetNumber());
						break;
					case Multiply:
						newTree = std::make_unique<OperationTree>((*left).GetNumber() * (*right).GetNumber());
						break;
					case Divide:
						newTree = std::make_unique<OperationTree>((*left).GetNumber() / (*right).GetNumber());
						break;
					}
					left.release();
					right.release();
				}
				else newTree = std::make_unique<OperationTree>(std::move(left), std::move(right), el.type);
				break;

			}

			stack.push_back(std::move(newTree));
			el = in.ParseNext();

		}
	}
	catch(int a){
		std::cout << "Bad input" << std::endl; return 1;
	}
	if (stack.size()!=1) { std::cout << "Bad input" << std::endl; return 1; }
	TreePtr tr= std::move(stack.back());

	//(*tr).printTree();
	MinMax mm = FindMinMax(*tr,start,end);
	tr.reset();
	//vypis
	std::cout << "min=" << mm.min << " max=" << mm.max << std::endl;


	std::string ff;
	std::cin >> ff;
	return 0;
}

