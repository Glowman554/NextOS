#ifndef ATA_H
#define ATA_H

extern "C" {
	#include <mem.h>
}

#include <stdint.h>
#include <driver/driver.h>
#include <port.h>
		
class AdvancedTechnologyAttachment: public Driver {
	private:
		bool master;
		Port16Bit dataPort;
		Port8Bit errorPort;
		Port8Bit sectorCountPort;
		Port8Bit lbaLowPort;
		Port8Bit lbaMidPort;
		Port8Bit lbaHiPort;
		Port8Bit devicePort;
		Port8Bit commandPort;
		Port8Bit controlPort;

		char* name;

		uint16_t bytesPerSector;

	public:
		AdvancedTechnologyAttachment(bool master, uint16_t portBase, char* name);
		~AdvancedTechnologyAttachment();

		void Read28(uint32_t sector, uint8_t* data, int count);
		void Write28(uint32_t sectorNum, uint8_t* data, uint32_t count);
		void Flush();

		virtual void activate();
		virtual bool is_presend();
		virtual char* get_name();
			
			
};

#include <fs/nextfs.h>	

#endif