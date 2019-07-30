## Inter Process Communication

def) IPC is a mechanism which allows processes to communicate each other and synchronize their actions. Processes can communicate with each other using these two ways.

1. Shared Memory
2. Message Passing



#### 1. Shared Memory

  Communication between processes using shared memory requires processes to share some variable and it completely depends on how programmer will implement it. -> Suppose process1 and process2 are executing simultaneously and they share some resource or use some information from other process. There are a few skills required. Like [semaphore](#semaphore), [mutual exclusion](#mutual-exclusion).



#### 2. Message Passing

  In this method, processes communicate with each other without using any kind of shared memory. They proceed as follow:

1. Establish a communication link(if a link already exists, no need to establish it again.)
2. Start exchanging messages using basic primitives. (send, receive)

  A standard message can have two parts : header and body. The header part is used for storing Message type, destination id, source id, message length and control information. 



###### message passing - through Communication Link.

It's need to implement communication link. A link has some capacity that determines the number of messages that can reside in it temporarily for which Every link has a queue associated with it which can be either of zero capacity or of bounded capacity or of unbounded capacity.

- **Direct Communication links** are implemented when the processes use specific process identifier
- **In-directed Communication links** is done via a shared mailbox, which consists of queue of messages.



###### message passing - through Exchanging the Messages.

A process that is blocked is one that waiting for some event such as a resource becoming available or the completion of an I/O operation. Blocking is considered synchronous and **blocking send** means the sender will be blocked until the message is received by receiver. Similarly, **blocking receive** has the receiver block until a message is available. The method that Non-blocking send and Blocking receive is mostly used.

- **Direct Communication links**, the process which want to communicate must explicitly name the recipient or sender of communication. In this method of communication, the communication link get established automatically, but one link can be used between one pair of the sender and receiver and one pair of sender and receiver should not possess more than one pair of link. The problem with this method of communication is that if the name of one process changes, this method will not work.
- **In-directed Communication links**, processes uses mailboxes (also referred to as ports) for sending and receiving messages. Each mailbox has a unique id and processes can communicate only if they share a mailbox. Link established only if processes share a common mailbox and a single link can be associated with many processes.



#### Method

IPC is set of interfaces, which is usually programmed in order for the programs to communicate between series of processes. This allows running programs concurrently in an Operating System. 

###### 1. Pipes (Same Process) -

This allows flow of data in one direction only. Analogous to simplex systems(keyboard). Data from the output is usually buffered until input process receives it which must have a common origin.

###### 2. Names Pipes (Different Processes) - 

This is a pipe with a specific name it can be used in processes that don't have a shared common process origin.

###### 3. Message Queuing -

This allows messages to be passed between processes using either a single queue or several message queue. This is managed by system kernel these messages are coordinated using an API.

###### 4. Semaphores -

This is used in solving problem associated with synchronization and to avoid race condition. These are integer values which are greater than or equal to 0.

###### 5. Shared memory - 

This allows the interchange of data through a defined area of memory. Semaphore values have to be obtained before data can get access to shared memory.

###### 6. Sockets -

This method is mostly used to communicate over a network between a client and a server. It allows for a standard connection which is computer and OS independent.



##### semaphore

- A semaphore is a variable or abstract data type used to control access to a common resource by multiple processes in a concurrent system such as a multitasking operating system. A semaphore is simply a variable. This variable is used to solve critical section problems and to achieve process synchronization in the multi processing environment.

##### mutual exclusion

- mutual exclusion is a property of concurrency control, which is instituted for the purpose of preventing race conditions: It is the requirement that one thread of execution never enters its critical section at the same time that another concurrent thread of execution enters its own critical section.