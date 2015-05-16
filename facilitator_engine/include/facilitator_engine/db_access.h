#include <string>
#include <vector>

#include <database_interface/db_class.h>

class QuestionAnswer : public database_interface::DBClass
{
public:
  database_interface::DBField<int> id_;
  database_interface::DBField<std::string> question_;
  database_interface::DBField<std::string> answer_;
  database_interface::DBField<std::string> question_type_;
  database_interface::DBField<std::string> evaluation_type_;
  database_interface::DBField<std::string> evaluation_logic_;

  QuestionAnswer() : 
    id_(database_interface::DBFieldBase::TEXT, 
                this, "id", "question_answer", true),
    question_(database_interface::DBFieldBase::TEXT, 
                        this, "question", "question_answer", true),
    answer_(database_interface::DBFieldBase::TEXT, 
                        this, "answer", "question_answer", true),
    question_type_(database_interface::DBFieldBase::TEXT, 
                        this, "question_type", "question_answer", true),
    evaluation_type_(database_interface::DBFieldBase::TEXT, 
                        this, "evaluation_type", "question_answer", true),
    evaluation_logic_(database_interface::DBFieldBase::TEXT, 
                        this, "evaluation_logic", "question_answer", true)
  {
    primary_key_field_ = &id_;

    fields_.push_back(&question_);
    fields_.push_back(&answer_);
    fields_.push_back(&question_type_);
    fields_.push_back(&evaluation_type_);
    fields_.push_back(&evaluation_logic_);

    setAllFieldsReadFromDatabase(true);
    setAllFieldsWriteToDatabase(true);
  }
};
