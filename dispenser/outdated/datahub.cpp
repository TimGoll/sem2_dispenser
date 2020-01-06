#include "datahub.hpp"

DataPoints::DataPoints(uint8_t max_data_points) {
	this->string_pos = 0;
	this->line_size = max_data_points * 2 * 8 + 1;

	this->data_line = new uint8_t[this->line_size];
	this->prefix = new uint8_t[5];
}

DataPoints::~DataPoints() {
	delete[] this->data_line;
	delete[] this->prefix;
}

void DataPoints::getPrefix(uint8_t* prefix) {
	strcpy(prefix, this->prefix);
}

void DataPoints::setPrefix(uint8_t* prefix) {
	strcpy(this->prefix, prefix);
}

void DataPoints::isPrefix(uint8_t* prefix) {
	return strcmp(this->prefix, prefix) == 0;
}

bool DataPoints::getNextDataPoint(uint8_t* type, uint8_t* data) {
	bool is_type = true;
	uint8_t type_pos = 0;
	uint8_t data_pos = 0;

	while (true) {
		// make sure data line is no empty string
		if (this->data_line[this->string_pos] == '\0')
			return false;

		// copy type tag
		if (is_type) {
			type[type_pos] = this->data_line[this->string_pos];
			type_pos++;

			if (type_pos == 3) {
				type[type_pos] = '\0';

				is_type = false;
			}
		} else {
			data[data_pos] = this->data_line[this->string_pos];
			data_pos++;

			if (this->data_line[this->string_pos + 1] == '\0' or this->data_line[this->string_pos + 1] == '#') {
				data[data_pos] = '\0';
				this->string_pos+=2;

				return true;
			}
		}

		this->string_pos++;
	}
}

void DataPoints::setNextDataPoint(uint8_t* type, uint8_t* data) {
	for (uint8_t i = 0; type[i] != '\0'; i++) {
		this->data_line[this->string_pos] = type[i];
		this->string_pos++;
	}

	for (uint8_t i = 0; data[i] != '\0'; i++) {
		this->data_line[this->string_pos] = data[i];
		this->string_pos++;
	}

	this->data_line[this->string_pos] = '#';
	this->string_pos++;
	this->data_line[this->string_pos] = '\0';
}

void DataPoints::getDataLine(uint8_t* line) {
	strcpy(line, this->prefix);
	strcat(line, this->data_line);
}

void DataPoints::setDataLine(uint8_t* line) {
	for (uint8_t i = 0; i < this->line_size + 4; i++) {
		if (i < 4) {
			this->prefix[i] = line[i];
		} else {
			this->data_line[i - 4] = line[i];
		}

		if (line[i] == '\0') {
			this->prefix[4] = '\0';
			this->data_line[i - 4] = '\0';

			break;
		}
	}
}

void DataPoints::addDataChar(uint8_t new_char) {
	this->data_line[this->string_pos] = new_char;
	this->string_pos++;
}

void DataPoints::completeDataLine() {
	this->data_line[this->string_pos] = '\0';
	this->string_pos = 0;

	// it is safe here to overwrite itself since there is a prefix offset
	this->setDataLine(this->data_line);
}

uint8_t DataPoints::getMaxLineSize() {
	return this->line_size;
}

DataHub::DataHub() : Callback(8) {
	this->card_available = SD.begin(CHIP_SELECT);
}

DataHub::~DataHub() {

}

void DataHub::readFromSD() {
	// make sure a SD card was found and is valid
	if (not this->card_available)
		return;

	// open the data file if existent
	File file = SD.open("data.txt", FILE_READ);

	// stop here if file was not found
	if (not file)
		return;

	// create a new data points structure
	DataPoints* dataPoints = new DataPoints(DATA_SIZE);

	// read the file stream until it ends
	while (file.available() > 0) {
		uint8_t new_char = (uint8_t) file.read();

		if (new_char == '\n') {
			dataPoints->completeDataLine();

			this->runCallback(dataPoints);
			
			delete dataPoints;

			dataPoints = new DataPoints(DATA_SIZE);
		} else {
			dataPoints->addDataChar(new_char);
		}
	}

	// when file end is reached, the last data point is reached as well
	dataPoints->completeDataLine();

	this->runCallback(dataPoints);

	delete dataPoints;

	// close the file
	file.close();
}

void DataHub::writeToSD() {
	// make sure a SD card was found and is valid
	if (not this->card_available)
		return;

	// remove existing data file
	SD.remove("data.txt");

	// create new data file
	File file = SD.open("data.txt", FILE_WRITE);

	// stop here if file was not found
	if (not file)
		return;

	// create a new data points structure
	DataPoints* dataPoints = new DataPoints(DATA_SIZE);

	uint8_t id = 0;
	while (true) {
		bool is_valid = this->runCallback(dataPoints, id);

		// make sure there is still a registered hook
		if (not is_valid)
			return;

		uint8_t data_line[dataPoints->getMaxLineSize()];

		dataPoints->getDataLine(data_line);

		file.println((char) data_line);

		delete dataPoints;
		dataPoints = new DataPoints(DATA_SIZE);

		id++;
	}

	delete dataPoints;

	// close the file
	file.close();
}
