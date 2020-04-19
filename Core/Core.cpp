#include "Core.h"
#include "Utils.h"

namespace Calculatrice::Core {
	std::vector<std::string> parse(std::string rawExpression)
	{
		// split expression
		std::vector<std::string> expression = Calculatrice::Utils::splitString(rawExpression, Calculatrice::Utils::Constants::delimiters);

		// loi de distributivitÚ
		// replace "-1" -> "+(-1)
		while (Calculatrice::Utils::vectorIncludes(expression, "-"))
		{
			int index = Calculatrice::Utils::firstIndexInVector(expression, "-");
			expression[index] = "+";
			expression[static_cast<size_t>(index) + 1] = "-" + expression[static_cast<size_t>(index) + 1];
		}

		// lois de prioritÚs de calcul
		struct priorityLawProperties
		{
			std::vector<int> multiplicationsIndexes;
			std::vector<int> divisionsIndexes;
			std::vector<int> exponentiationsIndexes;
		};

		priorityLawProperties props;

		if (Calculatrice::Utils::vectorIncludes(expression, "*"))
			props.multiplicationsIndexes = Calculatrice::Utils::indexesInVector(expression, "*");
		if (Calculatrice::Utils::vectorIncludes(expression, "/"))
			props.divisionsIndexes = Calculatrice::Utils::indexesInVector(expression, "/");
		if (Calculatrice::Utils::vectorIncludes(expression, "^")) // doit remonter au dessus de * et /
			props.exponentiationsIndexes = Calculatrice::Utils::indexesInVector(expression, "^");
			

		return expression;
	}

	double solve(std::string rawExpression) { return solve(parse(rawExpression)); }

	double solve(std::vector<std::string> expression)
	{
		if (expression.size() == 1) return std::stod(expression[0]);

		double result = 0;
		if (expression[1] == "+") result = std::stod(expression[0]) + std::stod(expression[2]);
		else if (expression[1] == "*") result = std::stod(expression[0]) * std::stod(expression[2]);
		else if (expression[1] == "/") result = std::stod(expression[0]) / std::stod(expression[2]);
		else if (expression[1] == "^") result = pow(std::stod(expression[0]), std::stod(expression[2]));

		std::vector<std::string> newExpression{ std::to_string(result) };
		std::copy(expression.begin() + 3, expression.end(), std::back_inserter(newExpression));

		return solve(newExpression);
	}
}