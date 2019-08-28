#pragma once
#include <map>
#include "bmloadout.h"
#include <iostream>
#include "helper_classes.h"

static inline void print_loadout(BMLoadout loadout)
{
	std::cout << "HEADER " << std::endl << "\tVersion: " << unsigned(loadout.header.version) << std::endl;
	std::cout << "\tSize in bytes: " << loadout.header.code_size << std::endl;
	std::cout << "\tCRC: " << unsigned(loadout.header.crc) << std::endl << std::endl;

	std::cout << "Blue is orange: " << (loadout.body.blue_is_orange ? "true" : "false") << std::endl;

	std::cout << "Blue: " << std::endl;

	for (auto body : loadout.body.blue_loadout)
	{
		std::cout << "\tSlot: " << unsigned(body.first) << ", ID: " << body.second.product_id << ", paint: " << unsigned(body.second.paint_index) << std::endl;
	}
	if (loadout.body.blueColor.should_override) {
		std::cout << "Color Primary (" << unsigned(loadout.body.blueColor.primary_colors.r) << ", " << unsigned(loadout.body.blueColor.primary_colors.g) << ", " << unsigned(loadout.body.blueColor.primary_colors.b) << ")";
		std::cout << " Secondary (" << unsigned(loadout.body.blueColor.secondary_colors.r) << ", " << unsigned(loadout.body.blueColor.secondary_colors.g) << ", " << unsigned(loadout.body.blueColor.secondary_colors.b) << ")";
	}

	if (!loadout.body.blue_is_orange)
	{
		std::cout << std::endl << "Orange: " << std::endl;
		for (auto body : loadout.body.orange_loadout)
		{
			std::cout << "\tSlot: " << unsigned(body.first) << ", ID: " << body.second.product_id << ", paint: " << unsigned(body.second.paint_index) << std::endl;
		}

		if (loadout.body.orangeColor.should_override) {
			std::cout << "Color Primary (" << unsigned(loadout.body.orangeColor.primary_colors.r) << ", " << unsigned(loadout.body.orangeColor.primary_colors.g) << ", " << unsigned(loadout.body.orangeColor.primary_colors.b) << ")";
			std::cout << " Secondary (" << unsigned(loadout.body.orangeColor.secondary_colors.r) << ", " << unsigned(loadout.body.orangeColor.secondary_colors.g) << ", " << unsigned(loadout.body.orangeColor.secondary_colors.b) << ")";
		}
	}
	std::cout << std::endl << std::endl;
}

static inline std::map<uint8_t, Item> read_items_from_buffer(BitBinaryReader<unsigned char>& reader)
{
	std::map<uint8_t, Item> items;
	int itemsSize = reader.ReadBits<int>(4); //Read the length of the item array
	for (int i = 0; i < itemsSize; i++)
	{
		Item option;
		int slotIndex = reader.ReadBits<int>(5); //Read slot of item
		int productId = reader.ReadBits<int>(13); //Read product ID
		bool isPaintable = reader.ReadBool(); //Read whether item is paintable or not

		if (isPaintable)
		{
			int paintID = reader.ReadBits<int>(6); //Read paint index
			option.paint_index = paintID;
		}
		option.product_id = productId;
		option.slot_index = slotIndex;
		items.insert_or_assign(slotIndex, option); //Add item to loadout at its selected slot
	}
	return items;
}

static inline RGB read_colors_from_buffer(BitBinaryReader<unsigned char>& reader)
{
	RGB col;
	col.r = reader.ReadBits<uint8_t>(8);
	col.g = reader.ReadBits<uint8_t>(8);
	col.b = reader.ReadBits<uint8_t>(8);
	return col;
}

static inline BMLoadout load(std::string loadoutString)
{
	BitBinaryReader<unsigned char> reader(loadoutString);
	BMLoadout loadout;

	/*
	Reads header
		VERSION (6 bits)
		SIZE_IN_BYTES (10 bits)
		CRC (8 BITS)
	*/
	loadout.header.version = reader.ReadBits<uint8_t>(6);
	loadout.header.code_size = reader.ReadBits<uint16_t>(10);
	loadout.header.crc = reader.ReadBits<uint8_t>(8);

	/* Verification (can be skipped if you already know the code is correct) */

	/*
	Calculate whether code_size converted to base64 is actually equal to the given input string
	Mostly done so we don't end up with invalid buffers, but this step is not required.
	*/
	int stringSizeCalc = ((int)ceil((4 * (float)loadout.header.code_size / 3)) + 3) & ~3;
	int stringSize = loadoutString.size();

	if (abs(stringSizeCalc - stringSize) > 6) //Diff may be at most 4 (?) because of base64 padding, but we check > 6 because IDK
	{
		//Given input string is probably invalid, handle
		std::cout << "Invalid input string size!";
		exit(0);
	}

	/*
	Verify CRC, aka check if user didn't mess with the input string to create invalid loadouts
	*/
	if (!reader.VerifyCRC(loadout.header.crc, 3, loadout.header.code_size))
	{
		//User changed characters in input string, items isn't valid! handle here
		std::cout << "Invalid input string! CRC check failed";
		exit(0);
	}

	//At this point we know the input string is probably correct, time to parse the body

	loadout.body.blue_is_orange = reader.ReadBool(); //Read single bit indicating whether blue = orange
	loadout.body.blue_loadout = read_items_from_buffer(reader); //Read loadout

	loadout.body.blueColor.should_override = reader.ReadBool(); //Read whether custom colors is on
	if (loadout.body.blueColor.should_override) {
		/* Read rgb for primary colors (0-255)*/
		loadout.body.blueColor.primary_colors = read_colors_from_buffer(reader);

		/* Read rgb for secondary colors (0-255)*/
		loadout.body.blueColor.secondary_colors = read_colors_from_buffer(reader);
	}

	if (loadout.body.blue_is_orange) //User has same loadout for both teams
	{
		loadout.body.orange_loadout = loadout.body.blue_loadout;
	}
	else {
		loadout.body.orange_loadout = read_items_from_buffer(reader);
		loadout.body.orangeColor.should_override = reader.ReadBool(); //Read whether custom colors is on
		if (loadout.body.blueColor.should_override) {
			/* Read rgb for primary colors (0-255)*/
			loadout.body.orangeColor.primary_colors = read_colors_from_buffer(reader);

			/* Read rgb for secondary colors (0-255)*/
			loadout.body.orangeColor.secondary_colors = read_colors_from_buffer(reader);
		}
	}

	return loadout;
}

static inline void write_loadout(BitBinaryWriter<unsigned char>& writer, std::map<uint8_t, Item> loadout)
{
	//Save current position so we can write the length here later
	const int amountStorePos = writer.current_bit;
	//Reserve 4 bits to write size later
	writer.WriteBits(0, 4);

	//Counter that keeps track of size
	int loadoutSize = 0;
	for (auto opt : loadout)
	{
		//In bakkesmod, when unequipping the productID gets set to 0 but doesn't
		//get removed, so we do this check here.
		if (opt.second.product_id <= 0)
			continue;
		loadoutSize++;
		writer.WriteBits(opt.first, 5); //Slot index, 5 bits so we get slot upto 31
		writer.WriteBits(opt.second.product_id, 13); //Item id, 13 bits so upto 8191 should be enough
		writer.WriteBool(opt.second.paint_index > 0); //Bool indicating whether item is paintable or not
		if (opt.second.paint_index > 0) //If paintable
		{
			writer.WriteBits(opt.second.paint_index, 6); //6 bits, allow upto 63 paints
		}
	}

	//Save current position of writer
	const int amountStorePos2 = writer.current_bit;
	writer.current_bit = amountStorePos;
	//Write the size of the loadout to the spot we allocated earlier
	writer.WriteBits(loadoutSize, 4); //Gives us a max of 15 customizable slots per team
	writer.current_bit = amountStorePos2; //Set back reader to original position
}

static inline void write_color(BitBinaryWriter<unsigned char>& writer, RGB color)
{
	writer.WriteBits(color.r, 8);
	writer.WriteBits(color.g, 8);
	writer.WriteBits(color.b, 8);
}


static inline std::string save(BMLoadout loadout)
{
	//Allocate buffer thats big enough
	BitBinaryWriter<unsigned char> writer(10000);
	writer.WriteBits(CURRENT_LOADOUT_VERSION, 6); //Write current version

	/*
	We write 18 empty bits here, because we determine size and CRC after writing the whole loadout
	but we still need to allocate this space in advance
	*/
	writer.WriteBits(0, 18);

	writer.WriteBool(loadout.body.blue_is_orange); //Write blue == orange?
	write_loadout(writer, loadout.body.blue_loadout);

	writer.WriteBool(loadout.body.blueColor.should_override); //Write override blue car colors or not

	if (loadout.body.blueColor.should_override)
	{
		write_color(writer, loadout.body.blueColor.primary_colors); // write primary colors RGB (R = 0-255, G = 0-255, B = 0-255)
		write_color(writer, loadout.body.blueColor.secondary_colors); //write secondary
	}

	if (!loadout.body.blue_is_orange)
	{
		write_loadout(writer, loadout.body.orange_loadout);
		writer.WriteBool(loadout.body.orangeColor.should_override);//Write override orange car colors or not
		if (loadout.body.orangeColor.should_override)
		{
			write_color(writer, loadout.body.orangeColor.primary_colors); //write primary
			write_color(writer, loadout.body.orangeColor.secondary_colors); //write secondary
		}
	}

	const int currentBit = writer.current_bit; //Save current location of writer

	int sizeInBytes = currentBit / 8 + (currentBit % 8 == 0 ? 0 : 1); //Calculate how many bytes are used
	writer.current_bit = 6; //Set writer to header (bit 6)
	writer.WriteBits(sizeInBytes, 10); //Write size
	writer.WriteBits(writer.CalculateCRC(3, sizeInBytes), 8); //Write calculated CRC
	writer.current_bit = currentBit; //Set writer back to original position
	return writer.ToHex();
}
