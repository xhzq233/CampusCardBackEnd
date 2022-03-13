# CampusCardBackEnd

模拟HUST智慧校园卡系统

## Current Project Structure

|        Name         |                 Function                 |
|:-------------------:|:----------------------------------------:|
|     FileManager     |             control files io             |
|        Card         |          store campusCard data           |
|       Account       |               store cards                |
|    BaseOperation    |            sortable operation            |
| CardManageOperation |      manage card related operations      |
|     Consumption     |   a data bound to a card and a window    |
|      DataStore      | store windows, accounts and consumptions |
|       Consume       |   construct consumption into dataStore   |
|     CardManage      |    construct card relative operation     |
|   CardDataAnalyze   |            analyze card data             |

```mermaid
graph TB

A((FileManager))
B((Window))
C((Card))
D((Account))

E((Consumption))
G((DataStore))
H((CardManage))
I((CardDataAnalyze))
J((BaseOperation))
K((CardManageOperation))
RC((RechargeOperation))


J-->E
J-->K
K-->RC

E-->G
B-->G
C-->D
D-->G
G-->H
G-->I

```

