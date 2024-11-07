#!/bin/dash

if [ $# -ne 2 ]
then
    echo "Usage: ./scraping_courses.sh <year> <course-prefix>"
    exit 1
fi

case "${1#[+-]}" in
    ''|*[!0-9]*)
        echo "./scraping_courses.sh: argument 1 must be an integer between 2019 and 2023"
        exit 1
        ;;
    *)
        :
        ;;
esac

if [ "$1" -lt 2019 ] || [ "$1" -gt 2023 ]
then
    echo "./scraping_courses.sh: argument 1 must be an integer between 2019 and 2023"
    exit 1
fi

YEAR=$1
AREA=$2

UNDERGRAD_URL="https://www.handbook.unsw.edu.au/api/content/render/false/query/+unsw_psubject.implementationYear:$YEAR%20+unsw_psubject.studyLevel:undergraduate%20+unsw_psubject.educationalArea:$AREA*%20+unsw_psubject.active:1%20+unsw_psubject.studyLevelValue:ugrd%20+deleted:false%20+working:true%20+live:true/orderby/unsw_psubject.code%20asc/limit/10000/offset/0"
POSTGRAD_URL="https://www.handbook.unsw.edu.au/api/content/render/false/query/+unsw_psubject.implementationYear:$YEAR%20+unsw_psubject.studyLevel:postgraduate%20+unsw_psubject.educationalArea:$AREA*%20+unsw_psubject.active:1%20+unsw_psubject.studyLevelValue:pgrd%20+deleted:false%20+working:true%20+live:true/orderby/unsw_psubject.code%20asc/limit/10000/offset/0"

# curl -sL $URL | jq -cr '.contentlets | .[] | "\(.code) \(.title)"' | sort

curl -sL $UNDERGRAD_URL curl -sL $POSTGRAD_URL | jq -cr '.contentlets | .[] | "\(.code) \(.title)"' | sort | uniq | tr -s ' '
