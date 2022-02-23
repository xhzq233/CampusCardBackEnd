# CampusCardBackEnd
 模拟HUST智慧校园卡系统

## Current Project Structure
|      Name       |                Function                |
|:---------------:|:--------------------------------------:|
|   FileManager   |            controls file io            |
|     Window      |          window id of a cafe           |
|      Card       |         stores campusCard data         |
|     Account     |              stores cards              |
|     Consume     |  a data bound to a card and a window   |
| ConsumeBuilder  | construct a consume data into database |
|    DataStore    |   stores windows, accounts, consumes   |
|   CardManage    |     manage card relative operation     |
| CardDataAnalyze |              analyze data              |

```mermaid
graph TB

A((FileManager))
B((Window))
C((Card))
D((Account))
E((Consumption))
F((ConsumeBuilder))
G((DataStore))
H((CardManage))
I((CardDataAnalyze))

G-->A
H-->A
I-->A
B-->E
D-->E
E-->G
F-->E
B-->G
C-->D
D-->G
G-->H
G-->I

```

