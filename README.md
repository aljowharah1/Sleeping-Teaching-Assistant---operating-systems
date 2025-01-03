# **Sleeping Teaching Assistant Simulation**

## **A Classic Concurrency Problem Implementation in C**
Explore process synchronization with semaphores, mutex locks, and multithreading to simulate a university teaching assistant and students during office hours.

---

## **Introduction**
This project is an implementation of the "Sleeping Teaching Assistant" problem, a well-known concurrency exercise in operating systems. It simulates a teaching assistant (TA) helping students during office hours:
- Students alternate between programming and seeking help.
- The TA sleeps when no students are waiting and wakes up when a student arrives.
- Up to 3 students can wait in chairs outside the office. If all chairs are occupied, new students must return later.

This project demonstrates the use of **POSIX threads**, **semaphores**, and **mutex locks** for synchronization in multithreaded programs

---

## **Diagram**

to be added 

---

## **Installation and Usage Instructions (For End-Users)**

### **1. Prerequisites**
- A Linux-based operating system or a POSIX-compatible environment.
- A C compiler (e.g., `gcc`).

### **2. Installation**
Clone the repository:
```bash
git clone https://github.com/aljowharah1/cs330.git
cd -------
```

### **3. Compilation**
Compile the code using:
```bash
gcc -o main main.c -lpthread
```

### **4. Running the Program**
Run the program:
```bash
./main
```

You will be prompted to enter the number of students:
```plaintext
Enter number of students: <number>
```

### **5. Example Output**
```plaintext
[Office Hours] Office hours are starting. TA will help students for 1 minutes.
[Student 1] Programming...
[Student 2] Programming...
[Student 1] Needs help, going to TA's office
[Student 1] Took seat 0.
Waiting students: [1] 1 [2] 0 [3] 0
[Student 1] Waking up the TA!
[TA] Helping student 1.
Waiting students: [1] 0 [2] 0 [3] 0
...
[Office Hours] Office hours are over. No more students will be accepted.
[TA] Office hours are over. No more students to help.
```

---

## **Installation and Usage Instructions (For Contributors)**

### **1. Pull the Repository**
Clone the repository for development:
```bash
git clone https://github.com/aljowharah1/cs330.git
```

### **2. Build for Development**
Compile the project using:
```bash
gcc -o main main.c -lpthread
```

### **3. Testing**
- Run test scenarios by adjusting the number of students and observing behavior.
- Modify the code to add new features or optimize the synchronization.

---

## **Contributor Expectations**

1. **Guidelines for Contributions**:
   - Create an issue before starting work.
   - Fork the repository and make changes in a separate branch.
   - Submit a pull request when ready for review.

2. **Code Style**:
   - Use proper indentation and comments.
   - Follow the existing structure for consistency.

3. **Testing**:
   - Test all changes thoroughly before submission.

---

## **Known Issues**
- Adding more than 3 seats currently requires manual adjustment of the array size and logic.
- The TA prints that it's helping a student before they actually start getting the help , making it look like  the TA is assisting another student before the previous one finishes 
- "took seat {0,1,2}" statement doesn't reflect the numbering of the chairs {1,2,3}

Feel free to report additional issues via the Issues tab.

---
