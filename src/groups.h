#include <utility>

#include "array.h"
#include "mystring.h"

struct GroupRecord {
String name;
String desc;
GroupRecord() : name("This is not a good name"), desc("Missing description") {}
GroupRecord(String n, String d) : name(std::move(n)), desc(std::move(d)) {}
bool operator==(const String& s) const {return name == s;}
bool operator!=(const String& s) const {return name != s;}
operator const String& () const {return name;}
friend std::ostream& operator<<(std::ostream& os, const GroupRecord& group) {return os << group.name;}
};

using ArrayOfGroupRecord = Array<GroupRecord>;
