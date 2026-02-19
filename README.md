# HOSTLE-MANAGEMENT-SYSTEM
A Robust System Architecture Case Study in C
A comprehensive, console-based administrative application designed to streamline hostel operations. This system manages complex student records and maintenance workflows using structured programming and persistent binary storage.

🚀 Key Features
🛡️ Administrative Control (Secure Portal)

       Role-Based Access: Password-protected login system ensuring only authorized personnel can modify student or hostel data.

       Comprehensive Student Management (CRUD):

 
                  Advanced Registration: Collects and validates 20+ fields including academic details and contact info.


                  Nested Update System: Intuitive menu-driven updates using a nested switch-case for precise data modification.


                  Soft Deletion: Implements an "Active Status" flag to preserve historical data integrity instead of permanent removal.

        Data Analysis & Reporting:


                  Search & Filter: Lookup students by ID, name, or filter specifically by campus.


                  In-Memory Sorting: Organizes records by name or room number for logistical planning.


                  Dashboard: Real-time summary view of the entire hostel state.


                  Export: Generates human-readable .txt reports for external use.


🎓 Student Portal (Self-Service)

                  Issue Reporting: Students can directly raise tickets for maintenance or hostel facilities.


                  Live Tracking: Real-time status monitoring (OPEN / IN_PROGRESS / RESOLVED) for all submitted tickets.

🏗️ Technical Architecture

     Data Management & Persistence

            The system prioritizes data durability. All information is preserved between sessions using direct binary file I/O.

                                   students.dat: Permanent storage for student records.

                                   tickets.dat: Persistent log of all maintenance issues.

     Implementation Details

                   Language: C.


                   Core Paradigms: Modular Design, Structured Programming.

                   Security & Validation:

                                   Duplicate ID prevention.

                                   Robust input handling with buffer clearing to prevent crashes from invalid user input.

                                   Formatted table-style UI for clean data visualization.

📂 Project Structure

             File Handling Module: Manages binary serialization and data loading.


             Admin Module: Contains core business logic for record management.


             Ticket Management: Handles the lifecycle of student-reported issues.


             Utility Module: Custom functions for robust integer and string input.

🛠️ Requirements & Setup

              Platform: Windows (Console-Based).

              Compiler: GCC (MinGW / MSYS2 recommended).

Developer: 000mahaswin000
