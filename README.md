This was my main C++ project in my second semester of Computer Science.

# Hospital Simulation

This repository contains a hospital simulation program written in C++. The program simulates the operation of a hospital, including the movement of patients, doctors and nurses, sending ambulances to interventions and conducting varius medical services.

## Requirements

To run the simulation, you need to have the following:

- C++ compiler that supports C++11 or later
- JSON file containing the hospital configuration
- Output file to store the simulation results

## Dependencies

The hospital simulation program depends on the following libraries:

- vcpkg - you need to have it to properly interpret json file

## Usage

The main file for the hospital simulation is `main.cpp`. The program accepts command-line arguments with the following format:

```
./hospital_simulation <json_file> <output_file> <iterations> <simulation_wait_time>
```

- `<json_file>`: Path to the JSON file containing the hospital configuration.
- `<output_file>`: Path to the file where the simulation results will be stored.
- `<iterations>`: Number of iterations for the simulation to run.
- `<simulation_wait_time>`: Time (in seconds) to wait between simulation steps.

Note: Ensure that the JSON file is properly formatted and contains the necessary information for the hospital configuration. There is an example of proper json file you could use to run this simulation.

## Running the Simulation

To run the hospital simulation, execute the following command:

```
./hospital_simulation hospital_config.json output.txt 100 1
```

This command will load the hospital configuration from `hospital_config.json`, run the simulation for 100 iterations with a wait time of 1 second between steps, and store the results in the `output.txt` file. Also the results will be visible in your terminal. 

## simulation.cpp

The provided code represents a simulation class for a hospital system. It simulates the operations and events that occur within the hospital. Let's go through the key aspects of the simulation.

The `Simulation` class has the following private member variables:
- `hospital`: A unique pointer to a `Hospital` object, which represents the hospital in the simulation.
- `iterations`: An unsigned integer representing the number of iterations or time steps the simulation will run.
- `messages`: A vector of strings used to store messages or events occurring during the simulation.
- `waitTime`: An unsigned short representing the wait time in seconds between each iteration of the simulation.
- `outputFileName`: A string representing the name of the output file where simulation messages will be written.
- `dateTime`: A `std::chrono::system_clock::time_point` representing the current date and time in the simulation.
- `randomNumberGenerator`: An instance of the `RandomNumberGenerator` class used for generating random numbers.
- `randomPatientGenerator`: An instance of the `RandomPatientGenerator` class used for generating random patients.

The public member functions of the `Simulation` class include:
- `Simulation`: The constructor for the `Simulation` class, which initializes the member variables and sets the initial date and time.
- `run`: This function executes the simulation by running the main simulation loop for the specified number of iterations.
- `incrementDateTime`: Increments the simulation's date and time by 15 minutes.
- `getDateTime`: Retrieves the current date and time in the simulation as a formatted string.
- `printMessages`: Prints the messages accumulated during the simulation to the console.
- `writeMessagesToFile`: Writes the messages accumulated during the simulation to the output file.
- `newPatientInReception`: Simulates a new patient arriving at the hospital's reception and adds them to the reception's patient queue.
- `patientCalled911`: Simulates a patient calling 911, generating a random patient and sending an ambulance if available.
- `checkForReturnedAmbulances`: Checks if any ambulances have returned from their interventions and processes them accordingly.
- `goThroughGeneralRooms`: Moves patients through the general rooms of each ward based on their conditions and planned services.
- `conductConsultations`: Simulates the process of conducting consultations for patients in the consultation rooms of each ward.
- `moveToGeneralRoom`: Moves patients from the hospital's reception to the general rooms if available or makes them wait in the reception.
- `conductTreatments`: Simulates the process of conducting treatments or operations for patients in the treatment rooms of each ward.

The `Simulation` class coordinates the flow of the simulation, making use of the various hospital components such as the `Hospital`, `Reception`, `AmbulanceDispatch`, `Ward`, `Room`, and different types of services (consultation and treatment/operation). It generates random patients and events, processes patient queues, assigns doctors and nurses, updates the simulation time, and handles the output of simulation events.

Overall, the simulation class provides a framework for modeling the behavior of a hospital system and can be further extended or modified to suit specific requirements and scenarios.

## other classes

There are many classes representing real-life objects like: ambulance, reception, patient, doctor, ward etc. I am not going to talk about them becouse I think the imlementation is pretty simple. I used some polimorphy and inheritant classes so that less code is needed when implementing another "human" into hospital. 

Additionally I implemented some helping classes like: random number generator or custom exceptions so the code is easier to understand. 

Code was tested in catch2 framework and you can look into the tests inside tests folder. 
