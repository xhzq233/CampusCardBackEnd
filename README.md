# CampusCardBackEnd

模拟HUST智慧校园卡系统

## Current Project Structure

|        Name         |                Function                 |
|:-------------------:|:---------------------------------------:|
|     FileManager     |            controls file io             |
|        Card         |         stores campusCard data          |
|       Account       |              stores cards               |
|    BaseOperation    |           sortable operation            |
| CardManageOperation |   management card related operations    |
|     Consumption     |   a data bound to a card and a window   |
|       Consume       | construct a consume data into dataStore |
|      DataStore      |  stores windows, accounts and consumes  |
|     CardManage      |     manage card relative operation      |
|   CardDataAnalyze   |              analyze data               |

```mermaid
graph TB

A((FileManager))
B((Window))
C((Card))
D((Account))

E((Consumption))
F((Consume))
G((DataStore))
H((CardManage))
I((CardDataAnalyze))
J((BaseOperation))
K((CardManageOperation))

J-->K
J-->E
B-->E
D-->E
E-->F
F-->G
B-->G
C-->D
D-->G
G-->H
G-->I

```

