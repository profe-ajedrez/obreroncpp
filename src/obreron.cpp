module;

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <sstream>
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <ctime>
#include <memory>

using std::size_t;
using std::string;
using std::vector;
using std::variant;
using std::optional;
using std::function;
using std::visit;

export module obreron;

export namespace obreron {

class SelectModel;
class Column;

class ObrException : public std::exception {
    private:
    char * message;

    public:
    ObrException(char * msg) : message(msg) {}
    
    char * what () {
        return message;
    }
};


std::string escape_string(const std::string &input) {
  std::string output;
  output.reserve(input.size());

  for (char c : input) {
    switch (c) {
    case '\'':
      output += "\\\'"; // Escapar comillas simples
      break;
    case '\"':
      output += "\\\""; // Escapar comillas dobles
      break;
    case '\\':
      output += "\\\\"; // Escapar barras invertidas
      break;
    case '\0':
      output += "\\0"; // Escapar caracteres nulos
      break;
    case '\n':
      output += "\\n"; // Escapar nueva línea
      break;
    case '\r':
      output += "\\r"; // Escapar retorno de carro
      break;
    default:
      output += c;
    }
  }

  return output;
}

// Clase que agrupa las diferentes partes de una consulta SQL
class QueryElements {
  friend SelectModel;
  std::vector<std::string> columns;
  std::vector<std::string> join_on_conditions;
  std::vector<std::string> where_conditions;
  std::vector<std::string> group_by_columns;
  std::vector<std::string> having_conditions;
  std::vector<std::string> order_by_columns;

public:
  void reset() {
    columns.clear();
    join_on_conditions.clear();
    where_conditions.clear();
    group_by_columns.clear();
    having_conditions.clear();
    order_by_columns.clear();
  }
};


using SQLValue = variant<int, double, string, bool, time_t>;

class SQLParam {

public:
    virtual ~SQLParam() = default;
    virtual SQLValue get_value() const = 0;
};


class IntParam : public SQLParam {
    int value;
public:
    IntParam(int v) : value(v) {}

    SQLValue get_value() const override {
        return value;
    }

    int int_value() const {
        return value;
    }
};

class DoubleParam : public SQLParam {
    double value;
public:
    DoubleParam(double v) : value(v) {}

    SQLValue get_value() const override {
        return value;
    }

    double double_value() const {
        return value;
    }
};


class StringParam : public SQLParam {
    string value;
public:
    StringParam(string v) : value(v) {}

    SQLValue get_value() const override {
        return value;
    }

    string string_value() const {
        return value;
    }
};

class BoolParam : public SQLParam {
    bool value;
public:
    BoolParam(bool v) : value(v) {}

    SQLValue get_value() const override {
        return value;
    }

    bool bool_value() const {
        return value;
    }
};

class DateParam : public SQLParam {
    time_t value;
public:
    DateParam(time_t v) : value(v) {}

    SQLValue get_value() const override {
        return value;
    }

    time_t bool_value() const {
        return value;
    }
};

class SQLParams {
    vector<std::shared_ptr<SQLParam>> params;

public:
    int bind(std::shared_ptr<SQLParam> param) {
        params.push_back(param);
        return params.size() - 1;
    }

    void visit(vector<int> indexes, function<void(std::shared_ptr<SQLParam>)> stepper) {
        auto l = params.size();

        for (size_t i = 0; i < indexes.size(); i++) {
            if (indexes[i] > 0 && indexes[i] < l) {
                stepper(params[indexes[i]]);
                continue;
            }

            throw ObrException("[Obreron] there is a param which index is non existent. You should check the params binded.");
        }
    }
};




} // namespace obreron

