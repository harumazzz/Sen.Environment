// **Kernel Declaration**

/**
 * Namespace representing the kernel functionalities within the system.
 * This namespace likely provides access to core functionalities.
 *
 * @version 3 - (Update this version number as the kernel evolves)
 */
declare namespace Sen {
	// **Shell Sub-Namespace**

	/**
	 * Namespace representing the shell environment within the kernel.
	 * This namespace likely provides access to shell-related features.
	 */
	declare namespace Shell {
		/**
		 * Callback function for processing shell commands.
		 *
		 * @param argument An array containing the command string as the first element
		 *                 and any optional arguments as subsequent elements.
		 *
		 * @returns A string representing the output or result of the command execution.
		 */
		export function callback(argument: string, ...other: Array<string>): Array<string>;
	}

	/**
	 * Namespace for kernel-related functionalities.
	 * This namespace likely provides access to core system operations.
	 */
	declare namespace Kernel {
		/**
		 * The current version number of the kernel.
		 */
		export function version(): number;

		export function test<T>(source: string): T;

		declare class Clock {
			public constructor();

			public start_safe(): void;

			public stop_safe(): void;

			public reset(): void;

			public duration(): number;

			public duration_as_seconds(): number;

			public is_started(): boolean;

			public is_stopped(): boolean;
		}

		/**
		 * Sub-namespace for XML processing functionalities within the kernel.
		 */
		declare namespace XML {
			/**
			 * Interface representing an XML document structure after deserialization.
			 * Properties can have any string key and unknown value types.
			 */
			export interface XMLDocument {
				[x: string]: unknown;
			}

			/**
			 * Deserializes an XML string into a corresponding JavaScript object structure.
			 *
			 * @param source The XML string to be parsed.
			 * @returns The parsed XML document as an object.
			 * @throws {Error} If the XML string is invalid.
			 */
			export function deserialize<T extends XMLDocument>(source: string): T;

			/**
			 * Deserializes an XML file into a corresponding JavaScript object structure.
			 *
			 * @param source The path to the XML file to be parsed.
			 * @returns The parsed XML document as an object.
			 * @throws {Error} If the file cannot be read or the XML is invalid.
			 */
			export function deserialize_fs<T extends XMLDocument>(source: string): T;

			/**
			 * Serializes a JavaScript object structure into an XML string.
			 *
			 * @param source The object to be serialized.
			 * @returns The corresponding XML string representation.
			 * @throws {Error} If the object cannot be serialized into valid XML.
			 */
			export function serialize<T extends XMLDocument>(source: T): string;

			/**
			 * Serializes a JavaScript object structure into an XML file.
			 *
			 * @param destination The path to the destination file for the XML data.
			 * @param xml The object to be serialized.
			 * @throws {Error} If the file cannot be written or the object cannot be serialized into valid XML.
			 */
			export function serialize_fs<T extends XMLDocument>(destination: string, xml: T): void;
		}

		/**
		 *  * Kernel language support
		 *  * This namespace provides functions for loading and accessing the language used by the program kernel.
		 *  * The kernel language typically contains messages, labels, and other UI elements.
		 *  */

		declare namespace Language {
			/**
			 *  * Loads the language data from the specified source file.
			 *  * @param source Path to the file containing language data. This is typically a JSON file
			 *  *        or a file format specific to your program.
			 *  * This function should be called during program initialization to load the language data.
			 *  * **Note:** Consider adding comments on how errors are handled if the source file cannot be loaded.
			 */

			export function read_language(source: string): void;

			/**
			 *  * Retrieves the translated string for the given key from the loaded language data.
			 *  * @param key The key to lookup in the language data.
			 *  * @returns The translated string if the key is found, otherwise returns the key itself
			 *  *          (potentially indicating a missing translation).
			 *  * **Note:** Consider adding comments on how to handle cases where the language data is not loaded yet.
			 */

			export function get(key: string): string;
		}

		/**
		 * JS Operating System
		 *
		 * This namespace provides functions for retrieving information about the current operating system.
		 */
		declare namespace OperatingSystem {
			/**
			 * Gets the name of the current operating system (e.g., "Windows", "macOS", "Linux").
			 *
			 * @returns The name of the current operating system as a string.
			 */
			export function current(): string;
		}

		/**
		 * JavaScript Array Buffer
		 *
		 * This namespace provides functions for working with ArrayBuffers in JavaScript.
		 */
		declare namespace ArrayBuffer {
			/**
			 * Fills an ArrayBuffer with random bytes.
			 *
			 * @param value The ArrayBuffer to fill with random bytes.
			 */
			export function random(value: ArrayBuffer): void;

			/**
			 * Compares two ArrayBuffers for equality.
			 *
			 * @param source The first ArrayBuffer to compare.
			 * @param destination The second ArrayBuffer to compare.
			 * @returns `true` if the two ArrayBuffers are equal, `false` otherwise.
			 *
			 * **Note:** This function only compares the contents of the ArrayBuffers, not their memory addresses.
			 * Two ArrayBuffers with identical contents but different memory locations will be considered equal.
			 */
			export function compareArrayBuffer(
				source: ArrayBuffer,
				destination: ArrayBuffer,
			): boolean;
		}

		/**
		 * JavaScript call OS
		 *
		 * This namespace provides functions for interacting with the operating system.
		 */
		declare namespace Process {
			/**
			 * Runs a command in the operating system shell.
			 *
			 * @param command The command to execute as a string.
			 *
			 * **Note:** Consider adding error handling for cases where the command fails to execute.
			 */
			export function run(command: string): void;

			/**
			 * Executes a command in the operating system shell and returns its output.
			 *
			 * @param command The command to execute as a string.
			 * @returns The output of the command as a string.
			 *
			 * **Note:** Consider adding error handling for cases where the command fails to execute.
			 */
			export function execute(command: string): string;

			/**
			 * Checks if a command exists in the system's PATH environment variable.
			 *
			 * @param command The command to check for.
			 * @returns True if the command exists in PATH, false otherwise.
			 */
			export function is_exists_in_path_environment(command: string): boolean;

			/**
			 * Get path in the system's PATH environment variable.
			 *
			 * @param command The command to check for.
			 * @returns string.
			 */
			export function get_path_environment(command: string): string;
		}

		/**
		 * JavaScript Image Support
		 *
		 * This namespace provides functions for loading, manipulating, and saving images.
		 */

		declare namespace Image {
			/**
			 * Scales an image by a specified percentage and saves the result to a new file.
			 *
			 * @param source The path to the source image file.
			 * @param destination The path to the destination file for the scaled image.
			 * @param percentage The percentage to scale the image by (e.g., 50 for half size).
			 */
			export function scale_fs(source: string, destination: string, percentage: number): void;

			/**
			 * Resizes an image to a specific size and saves the result to a new file.
			 *
			 * **Consider using scale_fs for consistency, as resizing typically involves scaling.**
			 *
			 * @param source The path to the source image file.
			 * @param destination The path to the destination file for the resized image.
			 * @param percentage The percentage to scale the image by (e.g., 50 for half size).
			 */
			export function resize_fs(
				source: string,
				destination: string,
				percentage: number,
			): void;

			/**
			 * Interface defining a rectangle with position and dimensions.
			 * Used for specifying regions to cut from an image.
			 */
			export interface Rectangle {
				x: bigint;
				y: bigint;
				width: bigint;
				height: bigint;
			}

			/**
			 * Interface extending Rectangle with a destination path for saving cut images.
			 */
			export interface RectangleFileIO extends Rectangle {
				destination: string;
			}

			/**
			 * Cuts multiple rectangular regions from an image and saves them to new files.
			 *
			 * @param source The path to the source image file.
			 * @param data An array of RectangleFileIO objects defining the regions to cut.
			 */
			export function cut_multiple_fs(source: string, data: Array<RectangleFileIO>): void;

			/**
			 * Asynchronous version of cut_multiple_fs for cutting multiple rectangular regions from an image.
			 *
			 * @param source The path to the source image file.
			 * @param data An array of RectangleFileIO objects defining the regions to cut.
			 */
			export function cut_multiple_fs_asynchronous(
				source: string,
				data: Array<RectangleFileIO>,
			): void;

			/**
			 * Loads an image from a file and returns an image object representation.
			 *
			 * **Note:** The specific structure of the returned image object might be
			 *         implementation-dependent. Refer to additional documentation for details.
			 *
			 * @param source The path to the image file to load.
			 * @returns An image object representing the loaded image.
			 */
			export function open(source: string): Kernel.ImageAdapter.Image;

			/**
			 * Writes an image object to a file.
			 *
			 * **Note:** The specific structure of the `image` object is expected to be
			 *         compatible with the implementation. Refer to additional documentation for details
			 *         on the expected format of the image object.
			 *
			 * @param destination The path to the destination file for the image.
			 * @param image The image object representing the image to save.
			 */
			export function write(destination: string, image: Kernel.ImageAdapter.Image): void;

			/**
			 * Joins multiple image objects into a single image object.
			 *
			 * This function assumes the image objects have compatible dimensions
			 * (e.g., same width and height) for proper joining.
			 *
			 * @param dimension An object defining the dimensions (width and height) of the joined image.
			 * @param data An array of image objects to be joined together.
			 * @returns A new image object representing the joined image.
			 */
			export function join(
				dimension: Kernel.ImageAdapter.BasicView,
				data: Array<Kernel.ImageAdapter.Image>,
			): Kernel.ImageAdapter.Image;

			/**
			 * Joins multiple image objects into a single image object.
			 *
			 * This function assumes the image objects have compatible dimensions
			 * (e.g., same width and height) for proper joining.
			 *
			 * @param dimension An object defining the dimensions (width and height) of the joined image.
			 * @param data An array of image objects to be joined together.
			 * @returns A new image object representing the joined image.
			 */
			export function join_extend(
				dimension: Kernel.ImageAdapter.BasicView,
				data: Array<Kernel.ImageAdapter.Image>,
			): Kernel.ImageAdapter.Image;
		}

		/**
		 * Declarations for NodeJS Path module
		 */
		declare namespace Path {
			/**
			 * Get the last portion of a path.
			 * @param source - The path
			 * @returns The file name
			 */
			export function basename(source: string): string;

			/**
			 * Get the directory name from a path.
			 * @param source - The path
			 * @returns The directory name
			 */
			export function dirname(source: string): string;

			/**
			 * Normalize a path by resolving '..' and '.' segments.
			 * @param source - The path to normalize
			 * @returns The normalized path
			 */
			export function normalize(source: string): string;

			/**
			 * Get the extension of a path.
			 * @param source - The path
			 * @returns The file extension
			 */
			export function extname(source: string): string;

			/**
			 * Generate the relative path from one path to another.
			 * @param from - The starting path
			 * @param to - The target path
			 * @returns The relative path from 'from' to 'to'
			 */
			export function relative(from: string, to: string): string;

			/**
			 * Get the base name of a path without the file extension.
			 * @param source - The path
			 * @returns The base name without extension
			 */
			export function base_without_extension(source: string): string;

			/**
			 * Get the full path of a file excluding the file extension.
			 * @param source - The path
			 * @returns The full path without extension
			 */
			export function except_extension(source: string): string;
		}

		/**
		 * Color for printing
		 */

		export type Color = 'red' | 'green' | 'cyan' | 'yellow' | 'default';

		/**
		 * Custom JSON Serialization and Deserialization
		 *
		 * This namespace provides functions for custom JSON serialization and deserialization.
		 * These functions may handle data types differently than the built-in `JSON.parse` and `JSON.stringify` functions.
		 *
		 * **Important Notes:**
		 * - Deserialization converts `int` values to `bigint` and `float/double` values to `number`.
		 * - Consider providing documentation or specifications for any custom data type handling.
		 */
		declare namespace JSON {
			/**
			 * Deserializes a JSON string into a JavaScript object.
			 *
			 * This function might perform custom data type conversions during deserialization
			 * (e.g., converting `int` to `bigint`). Refer to additional documentation for details.
			 *
			 * @param str The JSON string to deserialize.
			 * @returns An object of the inferred type `T`.
			 */
			export function deserialize<T>(str: string): T;

			/**
			 * Deserializes a JSON object from a file into a JavaScript object.
			 *
			 * This function reads the contents of a file containing JSON data and performs
			 * deserialization with potential custom data type conversions. Refer to additional
			 * documentation for details.
			 *
			 * @param source The path to the JSON file.
			 * @returns An object of the inferred type `T`.
			 */
			export function deserialize_fs<T>(source: string): T;

			/**
			 * Serializes a JavaScript object into a JSON string.
			 *
			 * This function allows specifying indentation and ASCII-only encoding options.
			 *
			 * @param obj The object to serialize.
			 * @param indent The number of spaces for indentation (optional).
			 * @param ensure_ascii Whether to ensure ASCII characters only (optional, defaults to false).
			 * @returns The serialized JSON string.
			 */
			export function serialize<T>(obj: T): string;

			/**
			 * Serializes a JavaScript object into a JSON file.
			 *
			 * This function writes the serialized JSON representation of an object to a file.
			 *
			 * @param destination The path to the destination file.
			 * @param obj The object to serialize.
			 * @param indent The number of spaces for indentation (optional).
			 * @param ensure_ascii Whether to ensure ASCII characters only (optional, defaults to false).
			 * @returns: (void) - This function doesn't return a value.
			 */
			export function serialize_fs<T>(destination: string, obj: T): void;
		}

		/**
		 * Kernel File System operations exported to Script.
		 * Note: Kernel file system functions are based on C++ Standard file system <fstream>.
		 * These functions must be implemented in the Kernel; Script only needs to call them.
		 */
		declare namespace FileSystem {
			/**
			 * Read the content of a file.
			 * @param source - The path of the file to read
			 * @returns The content of the file as a string
			 */
			export function read_file(source: string): string;

			/**
			 * Read the content of a file encoded with UTF-16LE.
			 * @param source - The path of the file to read
			 * @returns The content of the file as a string
			 */
			export function read_file_encode_with_utf16le(source: string): string;

			/**
			 * Write content to a file.
			 * @param destination - The path of the file to write
			 * @param content - The content to write to the file
			 */
			export function write_file(destination: string, content: string): void;

			/**
			 * Write content to a file encoded with UTF-16LE.
			 * @param destination - The path of the file to write
			 * @param content - The content to write to the file
			 */
			export function write_file_encode_with_utf16le(
				destination: string,
				content: string,
			): void;

			/**
			 * Create a directory.
			 * @param destination - The path of the directory to create
			 */
			export function create_directory(destination: string): void;

			/**
			 * Check if a path points to a file.
			 * @param source - The path to check
			 * @returns True if the path points to a file, otherwise false
			 */
			export function is_file(source: string): boolean;

			/**
			 * Check if a path points to a directory.
			 * @param source - The path to check
			 * @returns True if the path points to a directory, otherwise false
			 */
			export function is_directory(source: string): boolean;

			/**
			 * Read all files within a directory.
			 * @param source - The path of the directory to read
			 * @returns An array of file names within the directory
			 */
			export function read_directory(source: string): Array<string>;

			/**
			 * Read all files and directories within a directory.
			 * @param source - The path of the directory to read
			 * @returns An array of file and directory names within the directory
			 */
			export function read_current_directory(source: string): Array<string>;

			/**
			 * Read only files within a directory.
			 * @param source - The path of the directory to read
			 * @returns An array of file names within the directory
			 */
			export function read_directory_only_file(source: string): Array<string>;

			/**
			 * Read only directories within a directory.
			 * @param source - The path of the directory to read
			 * @returns An array of directory names within the directory
			 */
			export function read_directory_only_directory(source: string): Array<string>;

			/**
			 * File system operations such as rename, copy, and remove.
			 */
			/**
			 * Rename a file or directory.
			 * @param source - The path of the file or directory to rename
			 * @param destination - The new path or name
			 */
			export function rename(source: string, destination: string): void;

			/**
			 * Copy a file or directory.
			 * @param source - The path of the file or directory to copy
			 * @param destination - The destination path or directory
			 */
			export function copy(source: string, destination: string): void;

			/**
			 * Remove a file or directory.
			 * @param source - The path of the file or directory to remove
			 */
			export function remove(source: string): void;
		}

		/**
		 * JavaScript Native Handling Interface
		 *
		 * This namespace provides functions for interacting with JavaScript execution capabilities
		 * from within the current environment.
		 *
		 * **Important Notes:**
		 * - Usage of these functions might have implications for security and isolation.
		 * - Thoroughly review any security policies and guidelines before using them.
		 * - Consider the implications of executing untrusted code.
		 */
		declare namespace JavaScript {
			/**
			 * Evaluates a JavaScript file and returns the result.
			 *
			 * @param source The path to the JavaScript file to execute.
			 * @returns The result of the file execution, inferred as type `T`.
			 */
			export function evaluate_fs<T>(source: string): T;
		}

		/**
		 * Namespace for functionalities related to kernel support.
		 *
		 * This namespace likely interacts with the underlying system or a specific framework
		 * to provide functionalities related to various aspects supported by the kernel.
		 * In this case, it provides a nested `Texture` namespace for texture encoding and decoding.
		 */
		declare namespace Support {
			declare namespace Project {
				declare namespace StreamCompressedGroup {
					/**
					 * Check a Stream Compressed Group is compsite.
					 *
					 * @param source The path to the source SCG file to be check.
					 * @throws {Error} If there is an error during decoding.
					 */
					export function test_scg(source: string): boolean;

					/**
					 * Decodes an SCG file.
					 *
					 * @param source {string} Path to the source SCG file.
					 * @param destination {string} Path to the destination directory where the unpacked files will be written.
					 * @param setting A Setting object containing configuration options for the SCG custom process.
					 * @returns {void} No return value, function unpacks the group to the destination directory.
					 */
					export function decode_fs(
						source: string,
						destination: string,
						setting: Script.Support.Miscellaneous.Project.StreamCompressedGroup.Configuration.Setting,
					): void;

					/**
					 * Encode_fs a directory containing resources into an SCG file.
					 *
					 *
					 * @param source {string} Path to the source directory containing the resources to be packed.
					 * @param destination {string} Path to the destination SCG group file.
					 * @param setting A Setting object containing configuration options for the SCG custom process.
					 * @returns {void} No return value, function packs the resources into the destination SCG group file.
					 */
					export function encode_fs(
						source: string,
						destination: string,
						setting: Script.Support.Miscellaneous.Project.StreamCompressedGroup.Configuration.Setting,
					): void;
				}
				declare namespace ResourceStreamBundle {
					/**
					 * Unpacks an RSG bundle file to specific directory contains SCG files.
					 *
					 * @param source {string} Path to the source RSB bundle file.
					 * @param destination {string} Path to the destination directory where the unpacked files will be written.
					 * @param setting A Setting object containing configuration options for the RSB unpack custom process.
					 * @returns {void} No return value, function unpacks the bundle to the destination directory.
					 */
					export function unpack_fs(
						source: string,
						destination: string,
						setting: Script.Support.Miscellaneous.Custom.ResourceStreamBundle.Configuration.Setting,
					): void;

					/**
					 * Packs a directory containing resources into an RSB bundle file.
					 *
					 *
					 * @param source {string} Path to the source directory containing the resources to be packed.
					 * @param destination {string} Path to the destination RSB bundle file.
					 * @param setting A Setting object containing configuration options for the RSB pack custom process.
					 * @returns {void} No return value, function packs the resources into the destination RSB bundle file.
					 */
					export function pack_fs(
						source: string,
						destination: string,
						setting: Script.Support.Miscellaneous.Custom.ResourceStreamBundle.Configuration.Setting,
					): void;
				}
			}

			/**
			 * Namespace for marmalade manipulation functionalities.
			 *
			 * This namespace provides functions for encoding and decoding marmalade, likely
			 * utilizing the capabilities of the kernel or a specific framework.
			 */
			declare namespace Marmalade {
				declare namespace DZip {
					/**
					 * Unpack a DZip file.
					 *
					 * @param source {string} Path to the source file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function unpack_fs(source: string, destination: string): void;

					/**
					 * Pack a DZip file.
					 *
					 * @param source {string} Path to the source file to be encoded.
					 * @param destination {string} Path to the destination file where the encoded data will be written.
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function pack_fs(source: string, destination: string): void;
				}
			}

			/**
			 * Namespace for texture manipulation functionalities.
			 *
			 * This namespace provides functions for encoding and decoding textures, likely
			 * utilizing the capabilities of the kernel or a specific framework.
			 */
			declare namespace Texture {
				/**
				 * Encodes a source file into a texture with a specified format.
				 *
				 * @param source The path to the source file to be encoded.
				 * @param destination The path to the destination encoded texture file.
				 * @param format The texture format to use for encoding (likely defined in `Sen.Script.Support.Texture.Format`).
				 * @throws {Error} If there is an error during encoding.
				 */
				export function encode_fs(
					source: string,
					destination: string,
					format: bigint,
				): void;

				/**
				 * Decodes a texture file into a specified format with given dimensions.
				 *
				 * @param source The path to the source texture file to be decoded.
				 * @param destination The path to the destination decoded file.
				 * @param width The width of the decoded texture (in bigint).
				 * @param height The height of the decoded texture (in bigint).
				 * @param format The target format for the decoded texture (likely defined in `Sen.Script.Support.Texture.Format`).
				 * @throws {Error} If there is an error during decoding.
				 */
				export function decode_fs(
					source: string,
					destination: string,
					width: bigint,
					height: bigint,
					format: bigint,
				): void;
			}

			/**
			 * Namespace for functionalities related to PopCap support.
			 *
			 * This namespace likely interacts with functionalities specific to the PopCap game engine
			 * or development environment. It provides a nested `RTON` namespace for Reflection Object
			 * Notation (RTON) processing.
			 */
			declare namespace PopCap {
				declare namespace TextTable {
					export type Type = 'utf8-text' | 'utf16-text' | 'json-map' | 'json-array';

					export function convert_fs(
						source: string,
						destination: string,
						source_type: Type,
						destination_type: Type,
					);
				}

				/**
				 * Namespace for Reflection Object Notation (RTON) processing.
				 *
				 * This namespace provides functions for decoding, encrypting, decrypting, and
				 * combined encode-and-encrypt operations on files likely in RTON format,
				 * which is a PopCap specific format.
				 */
				declare namespace ReflectionObjectNotation {
					/**
					 * Decodes a source RTON file into a destination file.
					 *
					 * @param source The path to the source RTON file to be decoded.
					 * @param destination The path to the destination decoded file.
					 * @throws {Error} If there is an error during decoding.
					 */
					export function decode_fs(source: string, destination: string): void;

					/**
					 * Decrypts a source RTON file using a key and initialization vector (IV)
					 * into a destination file.
					 *
					 * @param source The path to the source RTON file to be decrypted.
					 * @param destination The path to the destination decrypted file.
					 * @param key The secret key used for decryption.
					 * @param iv The initialization vector used for decryption.
					 * @throws {Error} If there is an error during decryption.
					 */
					export function decrypt_fs(
						source: string,
						destination: string,
						key: string,
						iv: string,
					): void;

					/**
					 * Encrypts a source file using a key and initialization vector (IV)
					 * and writes it as an RTON file to the destination.
					 *
					 * @param source The path to the source file to be encrypted.
					 * @param destination The path to the destination encrypted RTON file.
					 * @param key The secret key used for encryption.
					 * @param iv The initialization vector used for encryption.
					 * @throws {Error} If there is an error during encryption.
					 */
					export function encrypt_fs(
						source: string,
						destination: string,
						key: string,
						iv: string,
					): void;

					/**
					 * Decrypts and then decodes a source RTON file using a key and initialization vector (IV)
					 * into a destination file in one operation.
					 *
					 * @param source The path to the source RTON file to be processed.
					 * @param destination The path to the destination decrypted and decoded file.
					 * @param key The secret key used for decryption.
					 * @param iv The initialization vector used for decryption.
					 * @throws {Error} If there is an error during decryption or decoding.
					 */
					export function decrypt_and_decode_fs(
						source: string,
						destination: string,
						key: string,
						iv: string,
					): void;

					/**
					 * Encodes and then encrypts a source file into an RTON file with a specified key and initialization vector (IV).
					 *
					 * @param source The path to the source file to be processed.
					 * @param destination The path to the destination encrypted RTON file.
					 * @param key The secret key used for encryption.
					 * @param iv The initialization vector used for encryption.
					 * @throws {Error} If there is an error during encryption or encoding.
					 */
					export function encode_and_encrypt_fs(
						source: string,
						destination: string,
						key: string,
						iv: string,
					): void;

					/**
					 * Encodes a source file into an RTON file (destination).
					 *
					 * This function synchronously encodes the contents of a source file into the PopCap RTON format
					 * and writes the encoded data to the specified destination file.
					 *
					 * @param source The path to the source file to be encoded.
					 * @param destination The path to the destination RTON file.
					 * @throws {Error} If there is an error during encoding.
					 */
					export function encode_fs(source: string, destination: string): void;
				}

				/**
				 * Zlib Support
				 *
				 * This namespace provides functions for compressing and uncompressing data using the zlib library.
				 */

				declare namespace Zlib {
					/**
					 * Uncompresses a file using the zlib library.
					 *
					 * @param source {string} Path to the source file to be uncompressed.
					 * @param destination {string} Path to the destination file where the uncompressed data will be written.
					 * @param use_64_bit_variant {boolean} Flag to indicate whether to use the 64-bit variant of zlib (if available).
					 * @returns {void} No return value, function writes uncompressed data to the destination file.
					 */
					export function uncompress_fs(
						source: string,
						destination: string,
						use_64_bit_variant: boolean,
					): void;

					/**
					 * Compresses a file using the zlib library.
					 *
					 * @param source {string} Path to the source file to be compressed.
					 * @param destination {string} Path to the destination file where the compressed data will be written.
					 * @param use_64_bit_variant {boolean} Flag to indicate whether to use the 64-bit variant of zlib (if available).
					 * @returns {void} No return value, function writes compressed data to the destination file.
					 */
					export function compress_fs(
						source: string,
						destination: string,
						use_64_bit_variant: boolean,
					): void;
				}

				/**
				 * Render Effects Support
				 *
				 * This namespace provides functions for decoding and encoding files related to render effects.
				 */

				declare namespace RenderEffects {
					/**
					 * Decodes a render effect file.
					 *
					 * @param source {string} Path to the source file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function decode_fs(source: string, destination: string): void;

					/**
					 * Encodes a render effect file.
					 *
					 * @param source {string} Path to the source file to be encoded.
					 * @param destination {string} Path to the destination file where the encoded data will be written.
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function encode_fs(source: string, destination: string): void;
				}

				/**
				 * New Type Object Notation Support
				 *
				 * This namespace provides functions for decoding and encoding files related to the Newton data format.
				 */

				declare namespace NewTypeObjectNotation {
					/**
					 * Decodes a Newton data file.
					 *
					 * @param source {string} Path to the source file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function decode_fs(source: string, destination: string): void;

					/**
					 * Encodes a Newton data file.
					 *
					 * @param source {string} Path to the source file to be encoded.
					 * @param destination {string} Path to the destination file where the encoded data will be written.
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function encode_fs(source: string, destination: string): void;
				}

				/**
				 * Crypt-Data Support
				 *
				 * This namespace provides functions for encrypting and decrypting files using the Crypt-Data format.
				 */

				declare namespace CryptData {
					/**
					 * Decrypts a Crypt-Data encrypted file.
					 *
					 * @param source {string} Path to the source file to be decrypted.
					 * @param destination {string} Path to the destination file where the decrypted data will be written.
					 * @param key {string} The secret key used for decryption.
					 * @returns {void} No return value, function writes decrypted data to the destination file.
					 */
					export function decrypt_fs(
						source: string,
						destination: string,
						key: string,
					): void;

					/**
					 * Encrypts a file using the Crypt-Data format.
					 *
					 * @param source {string} Path to the source file to be encrypted.
					 * @param destination {string} Path to the destination file where the encrypted data will be written.
					 * @param key {string} The secret key used for encryption.
					 * @returns {void} No return value, function writes encrypted data to the destination file.
					 */
					export function encrypt_fs(
						source: string,
						destination: string,
						key: string,
					): void;
				}

				/**
				 * CharacterFontWidget2 Support
				 *
				 * This namespace provides functions for decoding and encoding files related to the CharacterFontWidget2 component.
				 */

				declare namespace CharacterFontWidget2 {
					/**
					 * Decodes a CharacterFontWidget2 data file.
					 *
					 * @param source {string} Path to the source file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function decode_fs(source: string, destination: string): void;

					/**
					 * Encodes data for use with the CharacterFontWidget2 component.
					 *
					 * @param source {string} Path to the source file to be encoded.
					 * @param destination {string} Path to the destination file where the encoded data will be written.
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function encode_fs(source: string, destination: string): void;
				}

				/**
				 * Particles Support
				 *
				 * This namespace provides functions for decoding and encoding particle data files.
				 */

				declare namespace Particles {
					/**
					 * Denotes the target platform for file system operations.
					 */
					export type Platform = 'pc' | 'game-console' | 'phone-32' | 'phone-64' | 'tv';

					/**
					 * Decodes a particle data file.
					 *
					 * @param source {string} Path to the source file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function decode_fs(
						source: string,
						destination: string,
						platform: Platform,
					): void;

					/**
					 * Decodes and uncompresses a particle data file.
					 *
					 * **This function combines two functionalities: first it decompresses the source file,
					 * and then it decodes the decompressed data.**
					 *
					 * @param source {string} Path to the source file to be decoded and uncompressed.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					// export function uncompress_and_encode(source: string, destination: string): void;

					/**
					 * Encodes a particle data file.
					 *
					 * @param source {string} Path to the source file to be encoded.
					 * @param destination {string} Path to the destination file where the encoded data will be written.
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function encode_fs(
						source: string,
						destination: string,
						platform: Platform,
					): void;

					/**
					 * Encodes and compresses a particle data file.
					 *
					 * **This function combines two functionalities: first it encodes the source data,
					 * and then it compresses the encoded data.**
					 *
					 * @param source {string} Path to the source file to be encoded and compressed.
					 * @param destination {string} Path to the destination file where the compressed data will be written.
					 * @returns {void} No return value, function writes compressed data to the destination file.
					 */
					// export function encode_and_compress_fs(source: string, destination: string): void;

					/**
					 * Convert particles json to xml.
					 *
					 * @param source {string} Path to the source file.
					 * @param destination {string} Path to the destination file where the xml data will be written.
					 * @returns {void} No return value, function writes xml data to the destination file.
					 */
					export function to_xml(source: string, destination: string): void;

					/**
					 * Convert particles xml to json.
					 *
					 * @param source {string} Path to the source file.
					 * @param destination {string} Path to the destination file where the json data will be written.
					 * @returns {void} No return value, function writes json data to the destination file.
					 */
					export function from_xml(source: string, destination: string): void;
				}

				/**
				 * PlayerInfo Support
				 *
				 * This namespace provides functions for decoding and encoding player info data files.
				 */

				declare namespace PlayerInfo {
					/**
					 * Denotes the target platform for file system operations.
					 */
					// export type Platform = "pc" | "game-console" | "phone-32" | "phone-64" | "tv";

					/**
					 * Decodes a particle data file.
					 *
					 * @param source {string} Path to the source file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function decode_fs(source: string, destination: string): void;

					/**
					 * Encodes a particle data file.
					 *
					 * @param source {string} Path to the source file to be encoded.
					 * @param destination {string} Path to the destination file where the encoded data will be written.
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function encode_fs(source: string, destination: string): void;
				}

				/**
				 * Compiled Text Support
				 *
				 * This namespace provides functions for decoding and encoding compiled text files. Compiled text files are likely encrypted and may use a specific format.
				 */

				declare namespace CompiledText {
					/**
					 * Decodes a compiled text file.
					 *
					 * @param source {string} Path to the source file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @param key {string} The secret key used for decryption.
					 * @param iv {string} The initialization vector used for decryption.
					 * @param use_64_bit_variant {boolean} Flag to indicate whether to use the 64-bit variant of a specific library (if applicable).
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function decode_fs(
						source: string,
						destination: string,
						key: string,
						iv: string,
						use_64_bit_variant: boolean,
					): void;

					/**
					 * Encodes a text file into a compiled format.
					 *
					 * @param source {string} Path to the source file to be encoded.
					 * @param destination {string} Path to the destination file where the encoded data will be written.
					 * @param key {string} The secret key used for encryption.
					 * @param iv {string} The initialization vector used for encryption.
					 * @param use_64_bit_variant {boolean} Flag to indicate whether to use the 64-bit variant of a specific library (if applicable).
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function encode_fs(
						source: string,
						destination: string,
						key: string,
						iv: string,
						use_64_bit_variant: boolean,
					): void;
				}

				/**
				 * PopCap Animation Support
				 *
				 * This namespace provides functions for decoding and encoding PopCap Animation (PAM) files.
				 */

				declare namespace Animation {
					/**
					 * Decodes a PopCap Animation (PAM) file.
					 *
					 * @param source {string} Path to the source PAM file to be decoded.
					 * @param destination {string} Path to the destination file where the decoded data will be written.
					 * @returns {void} No return value, function writes decoded data to the destination file.
					 */
					export function decode_fs(source: string, destination: string): void;

					/**
					 * Encodes data into the PopCap Animation (PAM) format.
					 *
					 * @param source {string} Path to the source file to be encoded in PAM format.
					 * @param destination {string} Path to the destination PAM file where the encoded data will be written.
					 * @returns {void} No return value, function writes encoded data to the destination file.
					 */
					export function encode_fs(source: string, destination: string): void;

					/**
					 * Flash Conversion Support
					 *
					 * This namespace provides functions for converting animation data between PopCap and Flash formats.
					 */

					/**
					 * Converts a PopCap Animation (PAM) file to Flash XFL format.
					 *
					 * @param source {string} Path to the source PAM file to be converted.
					 * @param destination {string} Path to the destination directory where the converted XFL files will be written.
					 * @param resolution {bigint} The resolution to use for the generated XFL file (specific interpretation may depend on the implementation).
					 * @returns {void} No return value, function writes converted XFL files to the destination directory.
					 */
					export function to_flash(
						source: string,
						destination: string,
						resolution: bigint,
						has_label: boolean,
					): void;

					/**
					 * Converts a Flash XFL file to PopCap Animation (PAM) format.
					 *
					 * @param source {string} Path to the source directory containing the XFL file to be converted.
					 * @param destination {string} Path to the destination PAM file where the converted data will be written.
					 * @returns {void} No return value, function writes converted PAM data to the destination file.
					 */
					export function from_flash(
						source: string,
						destination: string,
						has_label: boolean,
					): void;

					/**
					 * Function to convert data from the project's format to Flash (likely for export)
					 * @param source  Path to the source data file
					 * @param destination Path to save the converted data in Flash format
					 * @param resolution Resolution for the converted data (likely in pixels)
					 */
					export function decode_and_to_flash(
						source: string,
						destination: string,
						resolution: bigint,
						has_label: boolean,
					): void;

					/**
					 * Function to convert data from Flash format to the project's format (likely for import)
					 * @param source  Path to the source data file in Flash format
					 * @param destination Path to save the converted data in the project's format
					 */
					export function from_flash_and_encode(
						source: string,
						destination: string,
						has_label: boolean,
					): void;

					/**
					 * Namespace for miscellaneous document, image, and sprite utilities
					 */
					declare namespace Miscellaneous {
						/**
						 * Interface representing the structure of a document
						 */
						export interface Document {
							/** Array of paths to sprites */
							sprite: Array<string>;
							/** Array of paths to images */
							image: Array<string>;
							/** Array of paths to media files */
							media: Array<string>;
							/** Array of action definitions (likely for user interactions) */
							action: Array<string>;
						}

						/**
						 * Type representing a transformation matrix (used for positioning and manipulation)
						 */
						export type Matrix = [number, number, number, number, number, number];

						/**
						 * Type representing a color with RGBA values
						 */
						export type Color = [number, number, number, number];

						/**
						 * Class encapsulating information about an image
						 */
						/**
						 * Interface encapsulating information about an image
						 */
						export interface Image {
							/** Name of the image */
							name: string;

							/** ID of the image (possibly unique within the document) */
							id: string;

							/** Transformation matrix for the image */
							transform: Matrix;
						}

						/**
						 * Interface encapsulating information about a sprite
						 */
						export interface Sprite {
							/** Name of the sprite */
							name: string;

							/** Link to the image data for the sprite */
							link: string;

							/** Transformation matrix for the sprite */
							transform: Matrix;

							/** Color of the sprite */
							color: Color;
						}

						/**
						 * Resizes a filesystem to a specified resolution.
						 *
						 * @param source - The path to the filesystem to resize.
						 * @param resolution - The target resolution of the filesystem.
						 *                    The unit depends on the specific filesystem type.
						 *
						 * @throws {Error} -  If an error occurs during resize operation.
						 *                   (Implementation specific)
						 */

						export function resize_fs(source: string, resolution: number): void;

						/**
						 * Function to save a document object to a specified location
						 * @param source  Path to the source document
						 * @param document The document object to be saved
						 */
						export function dump_document(source: string, document: Document): void;

						/**
						 * Function to save a document object to a specified location
						 * @param destination  Path to the destination document
						 * @param document The document object to be saved
						 */
						export function generate_document(
							destination: string,
							document: Document,
						): void;

						/**
						 * Function to create an image file based on an Image object
						 * @param destination Path to save the generated image file
						 * @param image  The Image object containing the image data
						 */
						export function generate_image(destination: string, image: Image): void;

						/**
						 * Function to create a sprite image file based on a Sprite object
						 * @param destination Path to save the generated sprite file
						 * @param sprite  The Sprite object containing the sprite data
						 */
						export function generate_sprite(destination: string, sprite: Sprite): void;
					}

					/**
					 * Namespace for potential conversion to/from a specific game engine or development tool (possibly Flash)
					 */
					declare namespace Miscellaneous {
						/**
						 * Function resize Flash xfl
						 * @param source  Path to the source data file
						 * @param resolution Resolution for the converted data (likely in pixels)
						 */
						export function resize_fs(source: string, resolution: bigint): void;
					}
				}

				/**
				 * RSB Patch Support
				 *
				 * This namespace provides functions for creating and applying RSB patches. RSB patches are likely related to the Roundtable on Sustainable Biomaterials (RSB) organization and might be used to update or modify files in an RSB-related context.
				 */

				declare namespace ResourceStreamBundlePatch {
					/**
					 * Creates an RSB patch file.
					 *
					 * This function likely generates a diff between the `before` and `after` files and stores it in the `patch` file.
					 *
					 * @param before {string} Path to the original file before the changes.
					 * @param after {string} Path to the file after the changes.
					 * @param patch {string} Path to the destination file where the RSB patch will be written.
					 * @returns {void} No return value, function writes the RSB patch data to the destination file.
					 */
					export function encode_fs(before: string, after: string, patch: string): void;

					/**
					 * Applies an RSB patch to a file.
					 *
					 * This function likely uses the `patch` file to modify the `before` file, resulting in a version closer to the `after` file.
					 *
					 * @param before {string} Path to the file to be patched.
					 * @param patch {string} Path to the RSB patch file containing the changes.
					 * @param after {string} Path to the file representing the expected outcome after applying the patch (optional).
					 * @returns {void} No return value, function modifies the `before` file based on the RSB patch.
					 */
					export function decode_fs(before: string, patch: string, after?: string): void;
				}

				/**
				 * PopCap Resource Stream Bundle (RSB) Support
				 *
				 * This namespace provides functions for unpacking, packing, and manipulating PopCap Resource Stream Bundles (RSB). RSB bundles are likely a container format used by PopCap games to store various resources like textures, audio, and data.
				 */

				declare namespace ResourceStreamBundle {
					/**
					 * Flag type used for compression settings in RSB packets.
					 *
					 * The specific meaning of each flag value (0n, 1n, 2n, 3n) is likely dependent on the implementation.
					 */
					export type CompressionFlags = 0n | 1n | 2n | 3n;

					export interface TextureDimension extends Record<string, unknown> {
						/** Width of the texture in pixels. */
						width: bigint;
						/** Height of the texture in pixels. */
						height: bigint;
					}

					/**
					 * Interface representing information about a texture in an RSB bundle.
					 */
					export interface TextureInfo extends Record<string, unknown> {
						/** Unique identifier for the texture. */
						//  id: bigint;
						/** Format of the texture data (interpretation depends on implementation). */
						format: bigint;
						/**  */
						dimension: TextureDimension;
						/** Number of bytes per scanline (pitch). */
						pitch: bigint;
						/** Number of bits per alpha channel (optional). */
						alpha_size?: bigint;

						scale?: bigint;
					}

					/**
					 * Interface representing information about a resource within a group in an RSB bundle.
					 */
					export interface ResourceStreamGroupResInfo extends Record<string, unknown> {
						/** Path to the resource within the bundle. */
						path: string;
						/** Information about the texture associated with the resource (optional). */
						additional?: TextureInfo;
					}

					/**
					 *
					 */
					export interface Category extends Record<string, unknown> {
						resolution: null | bigint;

						locale: null | string;
					}

					/**
					 * Interface representing information about a resource stream group in an RSB bundle.
					 */
					export interface ResourceStreamGroupInfo extends Record<string, unknown> {
						/** Category of the resource stream group (interpretation depends on implementation). Can be null. */
						category: null | Category;
						/** Compression flags used for the resources in the packet. */
						compression: CompressionFlags;
						/** Array of resources associated with the packet. */
						resource: Array<ResourceStreamGroupResInfo>;
					}

					/**
					 * Interface representing information about a resource stream group pool in an RSB bundle.
					 */
					export interface ResourceStreamGroupPool extends Record<string, unknown> {
						/** Indicates whether the pool is a composite pool (likely nested structure). */
						composite: boolean;
						/** Mapping of subgroup names to their corresponding resource stream group information. */
						subgroup: Record<string, ResourceStreamGroupInfo>;
					}

					/**
					 * Interface representing the manifest of an RSB bundle.
					 */
					export interface Manifest extends Record<string, unknown> {
						/** Version of the RSB bundle format. */
						version: bigint;
						/** Size of a PTXInfo structure in bytes. */
						texture_information_version: bigint;
						/** Mapping of group names to their corresponding resource stream group pool information. */
						group: Record<string, ResourceStreamGroupPool>;
					}

					/**
					 * Unpacks an RSB bundle file.
					 *
					 * This function extracts the contents of an RSB bundle to the specified destination directory.
					 *
					 * @param source {string} Path to the source RSB bundle file.
					 * @param destination {string} Path to the destination directory where the unpacked files will be written.
					 * @returns {void} No return value, function unpacks the bundle to the destination directory.
					 */
					export function unpack_fs(source: string, destination: string): void;

					// TODO : Add comment
					export function unpack_resource(source: string, destination: string): void;
					export function pack_resource(source: string, destination: string): void;
					export function unpack_cipher(source: string, destination: string): void;

					/**
					 * Unpacks an RSB bundle file and returns the manifest for modding purposes.
					 *
					 * This function extracts the contents of an RSB bundle to the specified destination directory and returns a representation of the bundle's manifest structure. The manifest can be used to understand the organization of the resources within the bundle and potentially modify it for creating mods.
					 *
					 * @param source {string} Path to the source RSB bundle file.
					 * @param destination {string} Path to the destination directory where the unpacked files will be written.
					 * @returns {Manifest} An object representing the manifest structure of the unpacked bundle.
					 */
					export function unpack_for_modding_fs(
						source: string,
						destination: string,
					): Manifest;

					/**
					 * Packs a directory containing unpacked resources into a new RSB bundle file.
					 *
					 * This function likely takes a directory structure containing modified or new resources and a manifest (potentially obtained from `unpack_for_modding_fs`) to create a new RSB bundle file.
					 *
					 * @param source {string} Path to the source directory containing the unpacked resources.
					 * @param destination {string} Path to the destination RSB bundle file.
					 * @param manifest {Manifest} The manifest structure representing the desired organization of the resources in the new bundle.
					 * @returns {void} No return value, function packs the resources and manifest into the destination RSB bundle file.
					 */
					export function pack(
						source: string,
						destination: string,
						manifest: Manifest,
					): void;

					/**
					 * Packs a directory containing resources into an RSB bundle file.
					 *
					 * This function likely takes a directory structure containing game resources and creates a corresponding RSB bundle file at the specified destination.
					 *
					 * @param source {string} Path to the source directory containing the resources to be packed.
					 * @param destination {string} Path to the destination RSB bundle file.
					 * @returns {void} No return value, function packs the resources into the destination RSB bundle file.
					 */

					export function pack_fs(source: string, destination: string): void;
				}

				/**
				 * PopCap Resource Stream Group Support
				 * This namespace groups functions and types related to PopCap Resource Stream Groups (RSG).
				 */

				declare namespace ResourceStreamGroup {
					/**
					 * Version of the RSG file format.
					 */
					export type Version = 3n | 4n; // Use bigint for large file sizes

					/**
					 * Flags indicating the compression applied to the resources within the RSG file.
					 * Multiple flags can be combined.
					 */
					export type CompressionFlags = 0n | 1n | 2n | 3n;

					export interface TextureDimension extends Record<string, unknown> {
						/** Width of the texture in pixels. */
						width: bigint;
						/** Height of the texture in pixels. */
						height: bigint;
					}

					/**
					 * Interface representing information about a texture in an RSB bundle.
					 */
					export interface TextureInfo extends Record<string, unknown> {
						/** Unique identifier for the texture. */
						//  id: bigint;
						/** Format of the texture data (interpretation depends on implementation). */
						format: bigint;
						/**  */
						dimension: TextureDimension;
					}

					/**
					 * Interface representing information about a resource within a group in an RSB bundle.
					 */
					export interface ResourceStreamGroupResInfo extends Record<string, unknown> {
						/** Path to the resource within the bundle. */
						path: string;
						/** Information about the texture associated with the resource (optional). */
						additional?: TextureInfo;
					}

					/**
					 * Interface defining the structure of an RSG file header.
					 */
					export interface Definition {
						version: Version; // Version of the RSG file format
						compression: CompressionFlags; // Compression flags used in the archive
						res: Array<ResourceStreamGroupResInfo>; // Array containing information for each resource in the file
					}
					/**
					 * JavaScript RSG Unpack method for file (modding purposes)
					 *
					 * Unpacks an RSG archive and extracts its contents to a specified directory.
					 *
					 * @param {string} source - Path to the source RSG file.
					 * @param {string} destination - Path to the directory where unpacked files should be placed.
					 * @returns {void} - Doesn't return any data, extracts the archive contents to the destination folder.
					 */
					export function unpack_fs(source: string, destination: string): void;

					/**
					 * JavaScript RSG Unpack method for file (header information)
					 *
					 * Unpacks an RSG archive and parses the header information.
					 *
					 * @param {string} source - Path to the source RSG file.
					 * @param {string} destination - (Optional) Path to the directory where unpacked files should be placed.
					 *                                 Can be omitted if only header information is needed.
					 * @returns {Definition} - Returns a Definition object containing information about the file format and resources within the archive.
					 */
					// export function unpack_modding(source: string, destination?: string): Definition;

					/**
					 * JavaScript RSG Pack method for file (modding purposes)
					 *
					 * Packs a directory of loose files into an RSG archive for modding purposes.
					 *
					 * @param {string} source - Path to the source directory containing the files to be packed.
					 * @param {string} destination - Path to the destination RSG archive file.
					 * @returns {void} - Doesn't return any data, creates a new RSG archive at the specified destination.
					 */
					export function pack_fs(source: string, destination: string): void;

					/**
					 * JavaScript RSG Pack method for file (custom header)
					 *
					 * Packs a directory of loose files into an RSG archive with custom header information.
					 *
					 * @param {string} source - Path to the source directory containing the files to be packed.
					 * @param {string} destination - Path to the destination RSG archive file.
					 * @param {Definition} packet_info - An object containing custom metadata about the archive format and resources.
					 * @returns {void} - Doesn't return any data, creates a new RSG archive with custom header information.
					 */
					// export function pack(source: string, destination: string, packet_info: Definition): void;
				}

				/**
				 * PopCap Package Support
				 * This namespace groups functions and types related to PopCap Package (PAK).
				 */

				declare namespace Package {
					/**
					 * Unpacks a file system archive from the source path to the destination directory.
					 *
					 * @param {string} source - The path to the archive file.
					 * @param {string} destination - The directory where the extracted files will be placed.
					 * @returns {void}
					 */
					export function unpack_fs(source: string, destination: string): void;

					/**
					 * Packs a directory and its contents into a file system archive at the destination path.
					 *
					 * @param {string} source - The directory to be archived.
					 * @param {string} destination - The path and filename of the resulting archive.
					 * @returns {void}
					 */
					export function pack_fs(source: string, destination: string): void;
				}

				/**
				 * PopCap Re-Animation
				 * This namespace groups functions and types related to PopCap Re-Animation (Reanim).
				 */

				declare namespace ReAnimation {
					/**
					 * Denotes the target platform for file system operations.
					 */
					export type Platform = 'desktop' | 'mobile32' | 'mobile64' | 'television';

					// Interface to define the structure of a single Reanim transform
					export interface ReanimTransform extends Record<string, unknown> {
						// X position of the transform
						x: number;
						// Y position of the transform
						y: number;
						// X-axis scale
						kx: number;
						// Y-axis scale
						ky: number;
						// X-axis shear
						sx: number;
						// Y-axis shear
						sy: number;
						// Rotation angle in degrees
						a: number;
						// Interpolation type (string representation)
						i: string;
						// Resource name for the transform (optional)
						resource?: string;
						// Second interpolation type (string representation) - optional
						i2?: string;
						// Second resource name for the transform (optional)
						resource2?: string;
						// Font name used for text transform (optional)
						font?: string;
						// Text content for text transform (optional)
						text?: string;
					}

					// Interface to define a Reanim track with its name and associated transforms
					export interface ReanimTrack extends Record<string, unknown> {
						// Name of the track
						name: string;
						// Array of ReanimTransform objects defining the animation for this track
						transforms: Array<ReanimTransform>;
					}

					// Interface to define overall Reanim information
					export interface ReanimInfo extends Record<string, unknown> {
						// Scale factor applied during animation playback
						do_scale: number;
						// Frames per second for the animation
						fps: number;
						// Array of ReanimTrack objects defining the animation tracks
						tracks: Array<ReanimTrack>;
					}

					// Supported platforms for Reanim packs
					export const PakPlatformX = ['PC', 'Xbox_360', 'TV'] as const;

					// Type alias for PakPlatform to enforce usage of valid platform names
					export type PakPlatform = (typeof PakPlatformX)[number];

					// Interface to define information about a Reanim pack
					export interface PakInfo extends Record<string, unknown> {
						// Platform the pack is intended for (from PakPlatform enum)
						pak_platform: PakPlatform;
						// Flag indicating whether to use windows path separators in the pack
						windows_path_separate: boolean;
						// Flag indicating whether to compress the pack using zlib
						zlib_compress: boolean;
					}

					/**
					 * Decodes a file system archive from the source path to the destination directory,
					 * targeting a specific platform.
					 *
					 * @param {string} source - The path to the archive file.
					 * @param {string} destination - The directory where the extracted files will be placed.
					 * @param {Platform} platform - The target platform for the decoded files.
					 * @returns {void}
					 */
					export function decode_fs(
						source: string,
						destination: string,
						platform: Platform,
					): void;

					/**
					 * Encodes a directory and its contents into a file system archive at the destination path,
					 * targeting a specific platform.
					 *
					 * @param {string} source - The directory to be archived.
					 * @param {string} destination - The path and filename of the resulting archive.
					 * @param {Platform} platform - The target platform for the encoded archive.
					 * @returns {void}
					 */
					export function encode_fs(
						source: string,
						destination: string,
						platform: Platform,
					): void;

					/**
					 * Convert reanim json to xml.
					 *
					 * @param source {string} Path to the source file.
					 * @param destination {string} Path to the destination file where the xml data will be written.
					 * @returns {void} No return value, function writes xml data to the destination file.
					 */
					export function to_xml(source: string, destination: string): void;

					/**
					 * Convert reanim xml to json.
					 *
					 * @param source {string} Path to the source file.
					 * @param destination {string} Path to the destination file where the json data will be written.
					 * @returns {void} No return value, function writes json data to the destination file.
					 */
					export function from_xml(source: string, destination: string): void;

					/**
					 * Decodes a file system archive from the source path to the destination directory,
					 * targeting a specific platform.
					 *
					 * @param {string} source - The path to the archive file.
					 * @param {string} destination - The directory where the extracted files will be placed.
					 * @param {Platform} platform - The target platform for the decoded files.
					 * @returns {void}
					 */
					export function decode_and_to_flash(
						source: string,
						destination: string,
						platform: Platform,
					): void;

					/**
					 * Encodes a directory and its contents into a file system archive at the destination path,
					 * targeting a specific platform.
					 *
					 * @param {string} source - The directory to be archived.
					 * @param {string} destination - The path and filename of the resulting archive.
					 * @param {Platform} platform - The target platform for the encoded archive.
					 * @returns {void}
					 */
					export function from_flash_and_encode(
						source: string,
						destination: string,
						platform: Platform,
					): void;

					/**
					 * Converts a file system from the source path to Flash format at the destination path.
					 *
					 * @param {string} source - The path to the source file system.
					 * @param {string} destination - The path and filename for the Flash output.
					 * @returns {void}
					 */
					export function to_flash(source: string, destination: string): void;

					/**
					 * Converts a file system from Flash format at the source path to a regular file system at the destination path.
					 *
					 * @param {string} source - The path to the source Flash file.
					 * @param {string} destination - The directory where the extracted files will be placed.
					 * @returns {void}
					 */
					export function from_flash(source: string, destination: string): void;
				}

				/**
				 * Resource Group Support
				 */

				declare namespace ResourceGroup {
					/**
					 * JavaScript Split Resource Group Support
					 *
					 * Splits a Resource Group archive into separate files based on resource types.
					 *
					 * @param {string} source - Path to the source Resource Group file.
					 * @param {string} destination - Path to the destination directory for split resources.
					 * @returns {void} - Doesn't return any data, creates separate files for each resource type in the destination directory.
					 */
					export function split_fs(source: string, destination: string): void;

					/**
					 * JavaScript Merge Resource Group Support
					 *
					 * Merges separate resource files back into a single Resource Group archive.
					 *
					 * @param {string} source - Path to the directory containing the split resource files.
					 * @param {string} destination - Path to the destination Resource Group file.
					 * @returns {void} - Doesn't return any data, creates a new Resource Group archive at the specified destination.
					 */
					export function merge_fs(source: string, destination: string): void;

					/**
					 * JavaScript Convert Resource Group Support
					 *
					 * Converts a Resource Group archive to a different layout or format.
					 *
					 * @param {string} source - Path to the source Resource Group file.
					 * @param {string} destination - Path to the destination Resource Group file (may be modified based on layout).
					 * @param {Sen.Script.Support.PopCap.ResourceGroup} layout - The target layout or format for the converted Resource Group.
					 * @returns {void} - Doesn't return any data, creates a new Resource Group archive with the specified layout.
					 */
					export function convert_fs(
						source: string,
						destination: string,
						layout: bigint,
					): void;

					export interface ResourceBasic extends Record<string, unknown> {
						/** Slot number associated with the resource */
						slot: bigint;
						/** Unique identifier for the resource */
						id: string;
						/** Path to the image file(s) within the Resource Group (can be a string or an array of strings for multiple images) */
						path: Array<string> | string;
						/** Resource type (always set to "Image" for this interface) */
						type: string;
					}

					/**
					 * Interface for Resource containing an Image
					 *
					 * Represents a resource within a Resource Group that contains an image file.
					 */
					export interface ResourceContainsImage extends ResourceBasic {
						/** Resource type (always set to "Image" for this interface) */
						type: 'Image';
					}

					/**
					 * Interface for Resource containing an Atlas
					 *
					 * Extends the ResourceContainsImage interface to include additional properties for atlas resources.
					 */
					export interface ResourceContainsAtlas extends ResourceContainsImage {
						/** Flag indicating whether the resource is part of an atlas */
						atlas: boolean;
						/** Width of the atlas image (if applicable) */
						width: bigint;
						/** Height of the atlas image (if applicable) */
						height: bigint;
					}

					/**
					 * Interface for Resource containing an Atlas
					 *
					 * Extends the ResourceContainsImage interface to include additional properties for atlas resources.
					 * An atlas resource is a single image file containing multiple smaller images (sprites).
					 */
					export interface ResourceContainsAtlas extends ResourceContainsImage {
						/** Flag indicating whether the resource is part of an atlas */
						atlas: boolean;
						/** Width of the atlas image (if applicable) */
						width: bigint;
						/** Height of the atlas image (if applicable) */
						height: bigint;
					}

					/**
					 * Interface for Resource containing a Sprite
					 *
					 * Extends the ResourceContainsImage interface to include additional properties for sprite resources.
					 * A sprite is a single image or animation cut out from a larger image (atlas).
					 */
					export interface ResourceContainsSprite extends ResourceContainsImage {
						/** Parent resource ID (if the sprite is part of a larger image) */
						parent: string;
						/** X-coordinate of the sprite within the image (if applicable) */
						ax: bigint;
						/** Y-coordinate of the sprite within the image (if applicable) */
						ay: bigint;
						/** Width of the sprite (if applicable) */
						aw: bigint;
						/** Height of the sprite (if applicable) */
						ah: bigint;
						/** Optional X-coordinate of the sprite within the rendered element (may not be present) */
						x?: bigint;
						/** Optional Y-coordinate of the sprite within the rendered element (may not be present) */
						y?: bigint;
						/** Optional number of columns in the sprite animation (may not be present) */
						cols?: bigint;
						/** Optional number of rows in the sprite animation (may not be present) */
						rows?: bigint;
					}

					/**
					 * Interface for a SubGroup within a Resource Group
					 *
					 * Represents a subgroup within a Resource Group that contains an image file.
					 * A subgroup can group related resources together.
					 */
					export interface ResourceSubgroup extends Record<string, unknown> {
						/** Unique identifier for the subgroup */
						id: string;
						/** Subgroup type (always set to "simple" for this interface) */
						type: 'simple';
						/** Parent subgroup ID (if applicable) */
						parent?: string;
						/** Resource reference string (meaning depends on the specific resource type) */
						res: string;
						/** Array of resources contained within the subgroup */
						resources: Array<ResourceContainsImage>;
					}

					/**
					 * Interface for a Resource Group
					 */

					export interface ResourceGroup {
						slot_count: bigint;
						resources: Array<ResourceSubgroup>;
					}
				}

				/**
				 * Res Info Support
				 */

				declare namespace ResInfo {
					/**
					 * JavaScript Split Res Info Support
					 *
					 * Splits a Res Info archive into separate files based on resource types.
					 *
					 * @param {string} source - Path to the source Res Info file.
					 * @param {string} destination - Path to the destination directory for split resources.
					 * @returns {void} - Doesn't return any data, creates separate files for each resource type in the destination directory.
					 */
					export function split_fs(source: string, destination: string): void {
						// Implementation for splitting a Res Info archive
					}

					/**
					 * JavaScript Merge Res Info Support
					 *
					 * Merges separate resource files back into a single Res Info archive.
					 *
					 * @param {string} source - Path to the directory containing the split resource files.
					 * @param {string} destination - Path to the destination Res Info file.
					 * @returns {void} - Doesn't return any data, creates a new Res Info archive at the specified destination.
					 */
					export function merge_fs(source: string, destination: string): void {
						// Implementation for merging separate resource files into a Res Info archive
					}

					/**
					 * JavaScript Convert Res Info Support
					 *
					 * Converts a Res Info archive to a different layout or format.
					 *
					 * @param {string} source - Path to the source Res Info file.
					 * @param {string} destination - Path to the destination Res Info file (may be modified based on layout).
					 * @param {ResInfo.Layout} layout - The target layout or format for the converted Res Info.
					 * @returns {void} - Doesn't return any data, creates a new Res Info archive with the specified layout.
					 */
					export function convert_fs(source: string, destination: string): void {
						// Implementation for converting a Res Info archive to a different layout
					}

					/**
					 * Type definition for Res Info layout (string or array)
					 */
					export type Layout = 'string' | 'array';

					/**
					 * Type definition for nullable values (either a type or null)
					 */
					export type Nullable<T> = null | T;

					/**
					 * Interface for common properties of resource data
					 */
					export interface Common extends Record<string, unknown> {
						/** Type of the resource data (implementation specific) */
						type: unknown;
					}

					/**
					 * Interface for Atlas resource data
					 */
					export interface Atlas extends Common {
						/** Resource type (always "Atlas" for this interface) */
						type: string;
						/** Mapping of resource names to AtlasBinding objects */
						packet: Record<string, AtlasBinding>;
					}

					/**
					 * Interface for binding information within an Atlas resource
					 */
					export interface AtlasBinding extends Record<string, unknown> {
						/** Type of the binding (implementation specific) */
						type: string;
						/** Path to the image file within the Atlas */
						path: string;
						/** Dimension information for the Atlas image */
						dimension: {
							/** Width of the Atlas image */
							width: bigint;
							/** Height of the Atlas image */
							height: bigint;
						};
						/** Mapping of sprite names to Sprite objects within the Atlas */
						data: Record<string, Sprite>;
					}

					/**
					 * Interface for default properties of a sprite
					 */
					export interface Default extends Record<string, unknown> {
						/** X-coordinate of the sprite within the image (if applicable) */
						ax: bigint;
						/** Y-coordinate of the sprite within the image (if applicable) */
						ay: bigint;
						/** Width of the sprite (if applicable) */
						aw: bigint;
						/** Height of the sprite (if applicable) */
						ah: bigint;
						/** X-coordinate of the sprite within the rendered element (optional) */
						x: bigint;
						/** Y-coordinate of the sprite within the rendered element (optional) */
						y: bigint;
						/** Number of columns in the sprite animation (optional) */
						cols?: bigint;
						/** Number of rows in the sprite animation (optional) */
						rows?: bigint;
					}

					/**
					 * Interface for Sprite resource data
					 *
					 * Extends the Record<string, unknown> to allow for additional properties specific to sprites.
					 */
					export interface Sprite extends Record<string, unknown> {
						/** Type of the sprite resource (implementation specific) */
						type: string;
						/** Path to the image file for the sprite */
						path: string;
						/** Default properties for the sprite (position, size, etc.) */
						default: Default;
					}

					/**
					 * Interface for Binary resource data
					 */
					export interface Binary extends Common {
						/** Resource type (always null for this interface) */
						type: null;
						/** Packet containing information about the binary data */
						packet: {
							/** Type of the data (always "File" for this interface) */
							type: 'File';
							/** Mapping of resource names to CommonBinaryData objects */
							data: Record<string, CommonBinaryData>;
						};
					}

					/**
					 * Interface for common properties of binary data
					 *
					 * Extends the Record<string, unknown> to allow for additional properties specific to binary data.
					 */
					export interface CommonBinaryData extends Record<string, unknown> {
						/** Type of the binary data (implementation specific) */
						type: string;
						/** Path to the binary data file */
						path: string;
						/** Flag indicating whether to force original vector symbol size (optional) */
						forceOriginalVectorSymbolSize?: boolean;
						/** Source path of the binary data (optional) */
						srcpath?: string;
					}

					/**
					 * Interface for CompositeShell resource data
					 *
					 * Extends the Record<string, unknown> to allow for additional properties specific to composite shells.
					 */
					export interface CompositeShell extends Record<string, unknown> {
						/** Flag indicating whether the resource is a composite shell */
						is_composite: boolean;
						/** Mapping of subgroup names to Common objects within the composite shell */
						subgroup: Record<string, Common>;
					}

					/**
					 * Interface for Structure resource data
					 *
					 * Extends the Record<string, unknown> to allow for additional properties specific to the resource structure.
					 */
					export interface Structure extends Record<string, unknown> {
						/** Layout specification for expanding resource paths (string or array) */
						expand_path: Layout;
						/** Mapping of group names to CompositeShell objects within the structure */
						groups: Record<string, CompositeShell>;
					}
				}
			}

			/**
			 * WWise Support
			 */

			declare namespace WWise {
				/**
				 * WWise Sound Bank Support
				 */
				declare namespace SoundBank {
					/**
					 * JavaScript function to decode a WWise sound bank file
					 *
					 * Decodes a source WWise sound bank file into a specified destination format.
					 *
					 * @param {string} source - Path to the source WWise sound bank file.
					 * @param {string} destination - Path to the destination file for the decoded sound bank.
					 * @returns {void} - Doesn't return any data, creates a decoded sound bank file at the specified destination.
					 */
					export function decode_fs(source: string, destination: string): void;

					/**
					 * JavaScript function to encode a sound bank file into WWise format
					 *
					 * Encodes a source sound bank file into the WWise format and saves it to the specified destination.
					 *
					 * @param {string} source - Path to the source sound bank file.
					 * @param {string} destination - Path to the destination WWise sound bank file.
					 * @returns {void} - Doesn't return any data, creates a WWise sound bank file at the specified destination.
					 */
					export function encode_fs(source: string, destination: string): void;

					/**
					 * Hash WWise ID
					 * @param source - Source string
					 * @param destination - Destination UInteger32 Wrapper
					 */
					export function hash(source: string): bigint;
				}
			}
		}

		/**
		 * ImageAdapter Support
		 */

		declare namespace ImageAdapter {
			/**
			 * Interface for a basic view with width and height properties
			 *
			 * Extends the Record<string, unknown> to allow for additional properties specific to basic ImageAdapter.
			 */
			export interface BasicView extends Record<string, unknown> {
				/**
				 * Width of the image (in bigint)
				 */
				width: bigint;
				/**
				 * Height of the image (in bigint)
				 */
				height: bigint;
			}

			/**
			 * Interface for an image dimension with additional properties
			 *
			 * Extends the Structure interface to include image specific properties.
			 */
			export interface Image extends BasicView {
				/**
				 * Optional path to the source image file (if applicable).
				 */
				source_file?: string;
				/**
				 * Bit depth of the image data (in bigint).
				 */
				bit_depth: bigint;
				/**
				 * Color type of the image data (in bigint).
				 */
				color_type: bigint;
				/**
				 * Interlace type of the image data (in bigint).
				 */
				interlace_type: bigint;
				/**
				 * Number of image channels (in bigint).
				 */
				channels: bigint;
				/**
				 * Number of bytes per row in the image data (in bigint).
				 */
				rowbytes: bigint;
				/**
				 * ArrayBuffer containing the raw image data.
				 */
				data: ArrayBuffer;
			}
		}

		/**
		 * Interface for an image view for cutting purposes (separate definition)
		 */

		declare interface IView extends Record<string, unknown> {
			/**
			 * Number of x position.
			 */
			x: bigint;
			/**
			 * Number of y position.
			 */
			y: bigint;
			/**
			 * Number of width.
			 */
			width: bigint;
			/**
			 * Number of height.
			 */
			height: bigint;
		}

		export namespace Encoding {
			export namespace APNG {
				export interface Setting {
					/**
					 * delay_frames_list: Array<bigint>
					 *
					 * Stores an array of bigint values representing the delay
					 * (in milliseconds or some unit) between consecutive frames
					 * in the APNG animation.
					 */
					delay_frames_list: Array<bigint>;

					/**
					 * loop: bigint
					 *
					 * Stores a bigint value representing the number of times
					 * the APNG animation should loop.
					 *
					 * A value of 0 indicates infinite looping, while any positive
					 * value specifies the number of loops before stopping playback.
					 */
					loop: bigint;

					/**
					 * width: bigint
					 *
					 * Stores a bigint value representing the width of the APNG image
					 * in pixels.
					 */
					width: bigint;

					/**
					 * height: bigint
					 *
					 * Stores a bigint value representing the height of the APNG image
					 * in pixels.
					 */
					height: bigint;

					/**
					 * trim: boolean
					 *
					 * Indicates whether to trim any transparent pixels from the edges
					 * of the APNG frames.
					 *
					 * A value of true enables trimming, while false keeps all pixels.
					 */
					trim: boolean;
				}
			}
		}

		/**
		 * ImageView Class
		 *
		 * This class represents an image object with properties and methods for manipulation.
		 */

		declare class ImageView {
			// Private member for the internal image data (implementation specific)
			private _ImageView: ImageView;

			/**
			 * Constructor for the ImageView class
			 *
			 * @param {Kernel.ImageAdapter.Image} value - The initial image data for the ImageView object.
			 */
			public constructor(value: Kernel.ImageAdapter.Image);

			/**
			 * Getter for the image bit depth property
			 *
			 * @returns {bigint} - The bit depth of the image data (number of bits per pixel).
			 */

			public get bit_depth(): bigint;

			/**
			 * Setter for the image bit depth property
			 *
			 * Modifies the bit depth of the image data (number of bits per pixel).
			 *
			 * **Note:** Changing the bit depth might require additional processing to ensure data integrity.
			 *
			 * @param {bigint} value - The new bit depth value for the image data.
			 */

			public set bit_depth(value: bigint): void;

			/**
			 * Getter for the number of image channels
			 *
			 * @returns {bigint} - The number of channels in the image data (e.g., RGB has 3 channels).
			 */
			public get channels(): bigint;

			/**
			 * Setter for the number of image channels
			 *
			 * Modifies the number of channels in the image data.
			 *
			 * **Note:** Changing the number of channels might require additional processing to ensure data integrity and visual representation.
			 *
			 * @param {bigint} value - The new number of channels for the image data.
			 */
			public set channels(value: bigint): void;

			/**
			 * Getter for the image color type property
			 *
			 * @returns {bigint} - The color type of the image data (e.g., RGB, RGBA, grayscale).
			 */
			public get color_type(): bigint;

			/**
			 * Setter for the image color type property
			 *
			 * Modifies the color type of the image data.
			 *
			 * **Note:** Changing the color type might require converting pixel data and potentially altering the visual appearance of the image.
			 *
			 * @param {bigint} value - The new color type value for the image data.
			 */
			public set color_type(value: bigint): void;

			/**
			 * Getter for the image interlace type property
			 *
			 * @returns {bigint} - The interlace type of the image data (progressive or interlaced).
			 */
			public get interlace_type(): bigint;

			/**
			 * Setter for the image interlace type property
			 *
			 * Modifies the interlace type of the image data.
			 *
			 * **Note:** Changing the interlace type might require re-arranging pixel data and may affect how the image is displayed.
			 *
			 * @param {bigint} value - The new interlace type value for the image data.
			 */
			public set interlace_type(value: bigint): void;

			/**
			 * Getter for the number of bytes per row in the image data
			 *
			 * @returns {bigint} - The number of bytes required to represent a single row of pixels in the image data.
			 */
			public get rowbytes(): bigint;

			/**
			 * Setter for the number of bytes per row in the image data
			 *
			 * Modifying the rowbytes value might affect how the image data is interpreted. It should be adjusted in conjunction with other image properties like width and channels.
			 *
			 * @param {bigint} value - The new number of bytes per row for the image data.
			 */
			public set rowbytes(value: bigint): void;

			/**
			 * Getter for the image width property
			 *
			 * @returns {bigint} - The width of the image in pixels.
			 */
			public get width(): bigint;

			/**
			 * Setter for the image width property
			 *
			 * Modifies the width of the image. This might require adjusting other properties like rowbytes and potentially reallocating image data.
			 *
			 * @param {bigint} value - The new width value for the image in pixels.
			 */
			public set width(value: bigint): void;

			/**
			 * Getter for the image height property
			 *
			 * @returns {bigint} - The height of the image in pixels.
			 */
			public get height(): bigint;

			/**
			 * Setter for the image height property
			 *
			 * Modifies the height of the image. This might require adjusting other properties like rowbytes and potentially reallocating image data.
			 *
			 * @param {bigint} value - The new height value for the image in pixels.
			 */
			public set height(value: bigint): void;

			/**
			 * Getter for the raw image data
			 *
			 * @returns {ArrayBuffer} - The raw image data as an ArrayBuffer. Caution: Modifying the contents of this buffer directly might corrupt the image.
			 */
			public get data(): ArrayBuffer;

			/**
			 * Setter for the raw image data
			 *
			 * Sets the raw image data from an ArrayBuffer. The expected format and size of the data should be compatible with the image properties like width, height, channels, etc.
			 *
			 * **Note:** Setting the data directly might bypass internal image format validation. Ensure the provided data adheres to the expected format.
			 *
			 * @param {ArrayBuffer} value - The new raw image data as an ArrayBuffer.
			 */
			public set data(value: ArrayBuffer): void;

			/**
			 * Static method to cut a portion of an image
			 *
			 * @param {ImageView} image - The image to cut from.
			 * @param {IView} area - The area definition for the portion to be cut.
			 * @returns {ImageView} - A new ImageView object containing the cut portion of the image.
			 */
			public static cut(image: ImageView, area: IView): ImageView;

			/**
			 * Static method to scale an image by a percentage
			 *
			 * @param {ImageView} image - The image to scale.
			 * @param {number} percentage - The percentage to scale the image by (positive for zoom-in, negative for zoom-out).
			 * @returns {ImageView} - A new ImageView object containing the scaled image.
			 */
			public static scale(image: ImageView, percentage: number): ImageView;

			/**
			 * Static method to resize an image by a percentage (likely similar to scale)
			 *
			 * Documentation for the specific behavior of resize vs scale is recommended.
			 *
			 * @param {ImageView} image - The image to resize.
			 * @param {number} percentage - The percentage to resize the image by.
			 * @returns {ImageView} - A new ImageView object containing the resized image.
			 */
			public static resize(image: ImageView, percentage: number): ImageView;

			/**
			 * Static method to rotate an image by a percentage (likely not applicable)
			 *
			 * Documentation for the specific behavior of rotate is recommended,
			 * as rotating an image by a percentage might not be intuitive.
			 *
			 * @param {ImageView} image - The image to rotate.
			 * @param {number} percentage - The percentage to rotate the image by.
			 * @returns {ImageView} - A new ImageView object containing the rotated image.
			 */
			public static rotate(image: ImageView, percentage: number): ImageView;

			/**
			 * Static method to read an image from a file
			 *
			 * @param {string} source - The path to the image file to read.
			 * @returns {ImageView} - A new ImageView object containing the loaded image data.
			 */
			public static read_fs(source: string): ImageView;

			/**
			 * Static method to write an image to a file
			 *
			 * @param {string} destination - The path to the destination file for writing the image.
			 * @param {ImageView} image - The image object to write to the file.
			 * @returns {void} - Doesn't return any data, writes the image data to the specified file.
			 */
			public static write_fs(destination: string, image: ImageView): void;

			/**
			 * Creates a new Kernel.ImageView instance.
			 *
			 * @param {bigint} width - The desired width of the image in pixels.
			 * @param {bigint} height - The desired height of the image in pixels.
			 * @param {ArrayBuffer} color - An ArrayBuffer containing raw pixel data in a format supported by the Kernel framework.
			 * @returns {Kernel.ImageView} A new, initialized Kernel.ImageView object.
			 */
			public static instance(
				width: bigint,
				height: bigint,
				color: ArrayBuffer,
			): Kernel.ImageView;
		}

		/**
		 * Kernel Boolean Wrapper Class
		 *
		 * This class likely provides a wrapper around the native JavaScript boolean type,
		 * potentially offering additional functionalities or behaviors specific to the Kernel framework.
		 */
		declare class Boolean {
			/**
			 * Internal empty class of Boolean
			 */

			private _Boolean: Boolean;
			/**
			 * Internal storage for the boolean value
			 */
			private _value: boolean;

			/**
			 * Getter for the internal boolean value
			 *
			 * @returns {boolean} - The current boolean value.
			 */
			public get value(): boolean;

			/**
			 * Setter for the internal boolean value
			 *
			 * @param {boolean} value - The new boolean value to set.
			 */
			public set value(value: boolean): void;

			/**
			 * Constructor for the Kernel Boolean class
			 *
			 * Likely initializes the internal value with the provided boolean argument.
			 *
			 * @param {boolean} value - The initial boolean value for the object.
			 */
			public constructor(value: boolean): void;
		}

		/**
		 * Kernel class must be implements this interface
		 */

		declare interface BaseWrapper {
			_value: BaseWrapper;
		}

		/**
		 * JsonWriter class
		 *
		 * This class provides methods for building JSON-formatted output in a structured way.
		 */
		declare class JsonWriter {
			/**
			 * An internal JsonWriter instance for managing nested structures.
			 * It's unclear why this is necessary at this point.
			 */
			private _JsonWriter: JsonWriter;

			/**
			 * Controls whether to write indented JSON for readability.
			 */
			private _writeIndent: boolean;

			/**
			 * Creates a new instance of the JsonWriter class.
			 */
			public constructor(): void;

			/**
			 * Clears any previously written JSON data.
			 */
			public clear(): void;

			/**
			 * Returns the generated JSON string.
			 *
			 * @returns The complete JSON string in compact or indented format.
			 */
			public toString(): string;

			/**
			 * Writes the start of a JSON array ('[').
			 */
			public writeStartArray(): void;

			/**
			 * Writes the end of a JSON array (']').
			 */
			public writeEndArray(): void;

			/**
			 * Writes the start of a JSON object ('{').
			 */
			public writeStartObject(): void;

			/**
			 * Writes the start of a JSON object ('}').
			 */
			public writeEndObject(): void;

			/**
			 * Writes a boolean value to the JSON output.
			 *
			 * @param value The boolean value to write (true or false).
			 */
			public writeBoolean(value: boolean): void;

			/**
			 * Writes a null value to the JSON output.
			 */
			public writeNull(): void;

			/**
			 * Writes a property name followed by a colon (:) in a JSON object.
			 *
			 * @param property_name The name of the property to write.
			 */
			public writePropertyName(property_name: string): void;

			/**
			 * Writes a string value to the JSON output.
			 *
			 * @param value The string value to write.
			 */
			public writeString(value: string): void;

			/**
			 * Writes a number value to the JSON output.
			 *
			 * @param value The number value to write.
			 */
			public writeNumber(value: number): void;

			/**
			 * Writes a BigInt value to the JSON output.
			 *
			 * @param value The BigInt value to write.
			 */
			public writeBigInt(value: bigint): void;
		}

		/**
		 * Kernel DataStreamView Class
		 *
		 * This class likely represents a view of a data stream within the Kernel framework.
		 * It provides methods for reading and writing data from the stream, managing positions,
		 * and potentially interacting with external sources like files.
		 */
		declare class DataStreamView {
			/**
			 * Internal storage for the DataStreamView object (might be redundant with other members)
			 */
			private _DataStreamView: DataStreamView;

			/**
			 * Current write position within the data stream
			 *
			 * This value indicates the byte offset where the next write operation will occur.
			 */
			public write_position: bigint;

			/**
			 * Current read position within the data stream
			 *
			 * This value indicates the byte offset from where the next read operation will occur.
			 */
			public read_position: bigint;

			/**
			 * Constructor for the DataStreamView class
			 *
			 * Creates a new DataStreamView object. Optionally, a source string can be provided
			 * (purpose of the string argument needs clarification based on current comments).
			 *
			 * @param {string} source (optional) - A source string to process (exact functionality unclear).
			 */
			public constructor(source: string): void;

			/**
			 * Create an instance of DataStreamView (constructor with 0 parameter)
			 */

			public static instance(): DataStreamView;

			/**
			 * Get the size of the current data stream in bytes
			 *
			 * @returns {bigint} - The size of the data stream in bytes.
			 */
			public size(): bigint;

			/**
			 * Get the capacity of the data stream in bytes
			 *
			 * This might indicate the maximum amount of data the stream can hold
			 * or the currently allocated memory for the stream.
			 *
			 * @returns {bigint} - The capacity of the data stream in bytes.
			 */
			public capacity(): bigint;

			/**
			 * Reserve additional space in the data stream
			 *
			 * This method likely increases the capacity of the stream by the specified amount of bytes.
			 *
			 * @param {bigint} value - The number of bytes to reserve.
			 */
			public reserve(value: bigint): void;
			/**
			 * Allocate additional space in the data stream
			 *
			 * This method likely increases the capacity of the stream by the specified amount of bytes.
			 *
			 * @param {bigint} value - The number of bytes to reserve.
			 */
			public allocate(value: bigint): void;

			/**
			 * Potentially data population from string (needs clarification)
			 *
			 * The purpose of the `fromString` method is unclear based on the current comment.
			 * It might be populating the data stream with content from the provided string,
			 * but further documentation is recommended.
			 *
			 * @param {string} value - The string to use for population (exact behavior unclear).
			 */
			public fromString(value: string): void;

			/**
			 * cast current DataView to JS ArrayBuffer
			 */

			public toArrayBuffer(): ArrayBuffer;

			/**
			 * cast current DataView to JS ArrayBuffer
			 */

			public getArrayBuffer(from: bigint, to: bigint): ArrayBuffer;

			/**
			 * Convert the current DataStreamView to a JavaScript Uint8Array
			 *
			 * This method creates a new Uint8Array containing the data from the current DataStreamView.
			 *
			 * @returns {Uint8Array} - A new Uint8Array representing the data stream content.
			 */
			public toUint8Array(): Uint8Array;

			/**
			 * Convert the current DataStreamView to a JavaScript ArrayBuffer
			 *
			 * This method creates a new ArrayBuffer containing the data from the current DataStreamView.
			 *
			 * @returns {ArrayBuffer} - A new ArrayBuffer representing the data stream content.
			 */
			public toArrayBuffer(): ArrayBuffer;

			/**
			 * Extract a sub-section of the data stream as a Uint8Array
			 *
			 * This method creates a new Uint8Array containing the data between the specified 'from' and 'to' byte offsets (inclusive).
			 *
			 * @param {bigint} from - The starting byte offset (inclusive).
			 * @param {bigint} to - The ending byte offset (inclusive).
			 * @returns {Uint8Array} - A new Uint8Array containing the extracted data.
			 */
			public getUint8Array(from: bigint, to: bigint): Uint8Array;

			/**
			 * Extract a sub-section of the data stream as an ArrayBuffer
			 *
			 * This method creates a new ArrayBuffer containing the data between the specified 'from' and 'to' byte offsets (inclusive).
			 *
			 * @param {bigint} from - The starting byte offset (inclusive).
			 * @param {bigint} to - The ending byte offset (inclusive).
			 * @returns {ArrayBuffer} - A new ArrayBuffer containing the extracted data.
			 */
			public getArrayBuffer(from: bigint, to: bigint): ArrayBuffer;

			/**
			 * Convert the current data stream content to a string
			 *
			 * This method encodes the data stream content as a string using a specific encoding
			 * (exact encoding might depend on the Kernel framework).
			 *
			 * @returns {string} - A string representation of the data stream content.
			 */
			public toString(): string;

			/**
			 * Write the data stream content to a file (likely Kernel-specific)
			 *
			 * This method likely writes the data stream content to a file at the specified destination path.
			 * The exact behavior and file format might be specific to the Kernel framework.
			 *
			 * @param {string} destination - The output file path.
			 */
			public out_file(destination: string): void;

			/**
			 * Write a Uint8 value to the data stream (with or without position)
			 *
			 * This method writes a single 8-bit unsigned integer value to the data stream.
			 * If a position is provided, it specifies the byte offset where the value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeUint8(value: bigint): void;

			/**
			 * Write a Uint16 value to the data stream (with or without position)
			 *
			 * This method writes a single 16-bit unsigned integer value to the data stream.
			 * If a position is provided, it specifies the byte offset where the value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeUint16(value: bigint): void;

			/**
			 * Write a Uint32 value to the data stream (with or without position)
			 *
			 * This method writes a single 32-bit unsigned integer value to the data stream.
			 * If a position is provided, it specifies the byte offset where the value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeUint32(value: bigint): void;

			/**
			 * Write a Uint64 value to the data stream (with or without position)
			 *
			 * This method writes a single 64-bit unsigned integer value to the data stream.
			 * If a position is provided, it specifies the byte offset where the value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeUint64(value: bigint): void;

			/**
			 * Write a Int8 value to the data stream (with or without position)
			 *
			 * Similar to writeUint8, this method writes a single 8-bit signed integer value.
			 * The value is interpreted as a signed integer and written accordingly.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeInt8(value: bigint): void;

			/**
			 * Write a Int16 value to the data stream (with or without position)
			 *
			 * Similar to writeUint16, this method writes a single 16-bit signed integer value.
			 * The value is interpreted as a signed integer and written accordingly.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeInt16(value: bigint): void;

			/**
			 * Write a Int32 value to the data stream (with or without position)
			 *
			 * Similar to writeUint32, this method writes a single 32-bit signed integer value.
			 * The value is interpreted as a signed integer and written accordingly.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeInt32(value: bigint): void;

			/**
			 * Write a Int64 value to the data stream (with or without position)
			 *
			 * Similar to writeUint64, this method writes a single 64-bit signed integer value.
			 * The value is interpreted as a signed integer and written accordingly.
			 *
			 * @param {bigint} value - The value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeInt64(value: bigint): void;

			/**
			 * Write an entire ArrayBuffer to the data stream (with or without position)
			 *
			 * This method writes the contents of the provided ArrayBuffer to the data stream.
			 * If a position is specified, it indicates the byte offset where the ArrayBuffer content should be written.
			 * Otherwise, the content is written at the current write position.
			 *
			 * @param {ArrayBuffer} value - The ArrayBuffer containing the data to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeArrayBuffer(value: ArrayBuffer): void;

			/**
			 * Write an entire Uint8Array to the data stream (with or without position)
			 *
			 * This method writes the contents of the provided Uint8Array to the data stream.
			 * If a position is specified, it indicates the byte offset where the Uint8Array content should be written.
			 * Otherwise, the content is written at the current write position.
			 *
			 * @param {Uint8Array} value - The Uint8Array containing the data to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeUint8Array(value: Uint8Array): void;

			/**
			 * Write a float value to the data stream (with or without position)
			 *
			 * This method writes a single-precision floating-point number to the data stream.
			 * If a position is specified, it indicates the byte offset where the float value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {number} value - The floating-point number to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeFloat(value: number): void;

			/**
			 * Write a double value to the data stream (with or without position)
			 *
			 * This method writes a double-precision floating-point number to the data stream.
			 * If a position is specified, it indicates the byte offset where the double value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {number} value - The floating-point number to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeDouble(value: number): void;

			/**
			 * Write a variable-length encoded signed 32-bit integer to the data stream (with or without position)
			 *
			 * This method writes a 32-bit signed integer using a variable-length encoding scheme.
			 * The specific encoding scheme might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset where the encoded integer should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The integer value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeVarInt32(value: bigint): void;

			/**
			 * Write a variable-length encoded signed 64-bit integer to the data stream (with or without position)
			 *
			 * This method writes a 64-bit signed integer using a variable-length encoding scheme.
			 * The specific encoding scheme might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset where the encoded integer should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The integer value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeVarInt64(value: bigint): void;

			/**
			 * Write a zigzag-encoded 32-bit signed integer to the data stream (with or without position)
			 *
			 * This method writes a 32-bit signed integer using a zigzag encoding scheme.
			 * Zigzag encoding is a technique for encoding signed integers into unsigned integers
			 * to improve compression or storage efficiency.
			 * If a position is specified, it indicates the byte offset where the encoded integer should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The integer value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeZigZag32(value: bigint): void;

			/**
			 * Write a zigzag-encoded 64-bit signed integer to the data stream (with or without position)
			 *
			 * This method writes a 64-bit signed integer using a zigzag encoding scheme.
			 * Similar to writeZigZag32, this uses zigzag encoding for signed integers.
			 * If a position is specified, it indicates the byte offset where the encoded integer should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The integer value to write (as a bigint).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeZigZag64(value: bigint): void;

			/**
			 * Write a string to the data stream (with or without position)
			 *
			 * This method writes a string to the data stream, likely using a specific encoding.
			 * The exact encoding used (e.g., UTF-8, UTF-16) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset where the string data should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeString(value: string): void;

			/**
			 * Write a string to the data stream, potentially with a 4-byte prefix indicating string length (with or without position)
			 *
			 * This method likely writes a string to the data stream similar to writeString, but it might prepend
			 * a 4-byte unsigned integer indicating the string length before the actual string data.
			 * If a position is specified, it indicates the byte offset where the string data (including potential length prefix) should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringFourByte(value: string): void;

			/**
			 * Write a null value to the data stream (with or without position)
			 *
			 * This method likely writes a specific number of bytes (depending on the Kernel framework) to represent a null value.
			 * If a position is specified, it indicates the byte offset where the null value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value (unused) - This parameter might be optional or used for compatibility purposes.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeNull(value?: bigint): void;

			/**
			 * Write a boolean value to the data stream (with or without position)
			 *
			 * This method writes a boolean value (true or false) to the data stream.
			 * The specific representation of boolean values (e.g., 1/0 bytes) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset where the boolean value should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {bigint} value - The boolean value to write (cast to bigint - likely 0 or 1).
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeBoolean(value: bigint): void;

			/**
			 * Write a string to the data stream using a Uint8 array for length encoding (with or without position)
			 *
			 * This method writes a string to the data stream, potentially using a specific encoding
			 * (e.g., UTF-8, UTF-16). It precedes the string data with a 1-byte unsigned integer
			 * representing the string length.
			 * If a position is specified, it indicates the byte offset where the string data (including length prefix) should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringByUint8(value: string): void;

			/**
			 * Write a string to the data stream using a Uint16 array for length encoding (with or without position)
			 *
			 * Similar to writeStringByUint8, this method writes a string with a length prefix,
			 * but it uses a 2-byte unsigned integer for the length.
			 * If a position is specified, it indicates the byte offset where the string data (including length prefix) should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringByUint16(value: string): void;

			/**
			 * Write a string to the data stream using a Uint32 array for length encoding (with or without position)
			 *
			 * Similar to previous string writing methods, this method uses a 4-byte unsigned integer for the length prefix before the string data.
			 * If a position is specified, it indicates the byte offset where the string data (including length prefix) should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringByUint32(value: string): void;

			/**
			 * Write a string to the data stream using a Int8 array for length encoding (with or without position)
			 *
			 * This method writes a string to the data stream, potentially using a specific encoding
			 * (e.g., UTF-8, UTF-16). It precedes the string data with a 1-byte signed integer
			 * representing the string length.
			 * If a position is specified, it indicates the byte offset where the string data (including length prefix) should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringByInt8(value: string): void;

			/**
			 * Write a string to the data stream using a Int16 array for length encoding (with or without position)
			 *
			 * Similar to writeStringByUint8, this method writes a string with a length prefix,
			 * but it uses a 2-byte signed integer for the length.
			 * If a position is specified, it indicates the byte offset where the string data (including length prefix) should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringByInt16(value: string): void;

			/**
			 * Write a string to the data stream using a Uint32 array for length encoding (with or without position)
			 *
			 * Similar to previous string writing methods, this method might use a 4-byte unsigned integer for the length prefix before the string data.
			 * Documentation might not explicitly mention support for Int32 encoding.
			 * Check the documentation for confirmation.
			 * If a position is specified, it indicates the byte offset where the string data (including length prefix) should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringByInt32(value: string): void;

			/**
			 * Write a string to the data stream without any explicit length encoding (with or without position)
			 *
			 * This method writes a string to the data stream without prepending any information
			 * about the string length. The behavior of this method might depend on the Kernel framework
			 * and how it handles string termination. Use with caution.
			 * If a position is specified, it indicates the byte offset where the string data should be written.
			 * Otherwise, the value is written at the current write position.
			 *
			 * @param {string} value - The string value to write.
			 * @param {bigint} position (optional) - The byte offset for writing (defaults to write_position).
			 */
			public writeStringByEmpty(value: string): void;

			/**
			 * Read an unsigned 8-bit integer from the data stream (with or without position)
			 *
			 * This method reads a single byte from the data stream and interprets it as an unsigned 8-bit integer.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 1 byte.
			 *
			 * @returns {bigint} The unsigned 8-bit integer value read from the data stream.
			 */
			public readUint8(): bigint;

			/**
			 * Read an unsigned 8-bit integer from the data stream at a specific position
			 *
			 * Similar to readUint8, this method reads a single byte from the data stream and interprets it as an unsigned 8-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 8-bit integer value read from the data stream.
			 */
			public readUint8(position: bigint): bigint;

			/**
			 * Read an unsigned 16-bit integer from the data stream (with or without position)
			 *
			 * This method reads two bytes from the data stream and interprets them as an unsigned 16-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 2 bytes.
			 *
			 * @returns {bigint} The unsigned 16-bit integer value read from the data stream.
			 */
			public readUint16(): bigint;

			/**
			 * Read an unsigned 16-bit integer from the data stream at a specific position
			 *
			 * Similar to readUint16, this method reads two bytes from the data stream and interprets them as an unsigned 16-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 16-bit integer value read from the data stream.
			 */
			public readUint16(position: bigint): bigint;

			/**
			 * Read an unsigned 24-bit integer from the data stream (with or without position)
			 *
			 * This method reads three bytes from the data stream and interprets them as an unsigned 24-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * Less common than other integer sizes, check if this method is supported by the specific Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 3 bytes.
			 *
			 * @returns {bigint} The unsigned 24-bit integer value read from the data stream (if supported).
			 */
			public readUint24(): bigint;

			/**
			 * Read an unsigned 24-bit integer from the data stream at a specific position
			 *
			 * Similar to readUint24, this method reads three bytes from the data stream and interprets them as an unsigned 24-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 * Less common than other integer sizes, check if this method is supported by the specific Kernel framework.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 24-bit integer value read from the data stream (if supported).
			 */
			public readUint24(position: bigint): bigint;

			/**
			 * Read an unsigned 32-bit integer from the data stream (with or without position)
			 *
			 * This method reads four bytes from the data stream and interprets them as an unsigned 32-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 4 bytes.
			 *
			 * @returns {bigint} The unsigned 32-bit integer value read from the data stream.
			 */
			public readUint32(): bigint;

			/**
			 * Read an unsigned 32-bit integer from the data stream at a specific position
			 *
			 * Similar to readUint32, this method reads four bytes from the data stream and interprets them as an unsigned 32-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 32-bit integer value read from the data stream.
			 */
			public readUint32(position: bigint): bigint;

			/**
			 * Read an unsigned 64-bit integer from the data stream (with or without position)
			 *
			 * This method reads eight bytes from the data stream and interprets them as an unsigned 64-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 8 bytes.
			 *
			 * @returns {bigint} The unsigned 64-bit integer value read from the data stream.
			 */
			public readUint64(): bigint;

			/**
			 * Read an unsigned 64-bit integer from the data stream at a specific position
			 *
			 * Similar to readUint64, this method reads eight bytes from the data stream and interprets them as an unsigned 64-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 64-bit integer value read from the data stream.
			 */
			public readUint64(position: bigint): bigint;

			/**
			 * Read a signed 8-bit integer from the data stream (with or without position)
			 *
			 * This method reads a single byte from the data stream and interprets it as a signed 8-bit integer.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 1 byte.
			 *
			 * @returns {bigint} The signed 8-bit integer value read from the data stream.
			 */
			public readInt8(): bigint;

			/**
			 * Read a signed 8-bit integer from the data stream at a specific position
			 *
			 * Similar to readInt8, this method reads a single byte from the data stream and interprets it as a signed 8-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The signed 8-bit integer value read from the data stream.
			 */
			public readInt8(position: bigint): bigint;

			/**
			 * Read a signed 16-bit integer from the data stream (with or without position)
			 *
			 * This method reads two bytes from the data stream and interprets them as a signed 16-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 2 bytes.
			 *
			 * @returns {bigint} The signed 16-bit integer value read from the data stream.
			 */
			public readInt16(): bigint;

			/**
			 * Read a signed 16-bit integer from the data stream at a specific position
			 *
			 * Similar to readInt16, this method reads two bytes from the data stream and interprets them as a signed 16-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The signed 16-bit integer value read from the data stream.
			 */
			public readInt16(position: bigint): bigint;

			/**
			 * Read a signed 24-bit integer from the data stream (with or without position)
			 *
			 * This method reads three bytes from the data stream and interprets them as a signed 24-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * Less common than other integer sizes, check if this method is supported by the specific Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 3 bytes.
			 *
			 * @returns {bigint} The signed 24-bit integer value read from the data stream (if supported).
			 */
			public readInt24(): bigint;

			/**
			 * Read a signed 24-bit integer from the data stream at a specific position
			 *
			 * Similar to readInt24, this method reads three bytes from the data stream and interprets them as a signed 24-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 * Less common than other integer sizes, check if this code is supported by the specific Kernel framework.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The signed 24-bit integer value read from the data stream (if supported).
			 */
			public readInt24(position: bigint): bigint;

			/**
			 * Read a signed 32-bit integer from the data stream (with or without position)
			 *
			 * This method reads four bytes from the data stream and interprets them as a signed 32-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 4 bytes.
			 *
			 * @returns {bigint} The signed 32-bit integer value read from the data stream.
			 */
			public readInt32(): bigint;

			/**
			 * Read a signed 32-bit integer from the data stream at a specific position
			 *
			 * Similar to readInt32, this method reads four bytes from the data stream and interprets them as a signed 32-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The signed 32-bit integer value read from the data stream.
			 */
			public readInt32(position: bigint): bigint;

			/**
			 * Read a signed 64-bit integer from the data stream (with or without position)
			 *
			 * This method reads eight bytes from the data stream and interprets them as a signed 64-bit integer.
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 8 bytes.
			 *
			 * @returns {bigint} The signed 64-bit integer value read from the data stream.
			 */
			public readInt64(): bigint;

			/**
			 * Read a signed 64-bit integer from the data stream at a specific position
			 *
			 * Similar to readInt64, this method reads eight bytes from the data stream and interprets them as a signed 64-bit integer.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The signed 64-bit integer value read from the data stream.
			 */
			public readInt64(position: bigint): bigint;

			/**
			 * Read a 32-bit floating-point number from the data stream (with or without position)
			 *
			 * This method reads four bytes from the data stream and interprets them as a single-precision (32-bit) floating-point number (float).
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 4 bytes.
			 *
			 * @returns {number} The 32-bit floating-point number value read from the data stream.
			 */
			public readFloat(): number;

			/**
			 * Read a 32-bit floating-point number from the data stream at a specific position
			 *
			 * Similar to readFloat, this method reads four bytes from the data stream and interprets them as a float.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {number} The 32-bit floating-point number value read from the data stream.
			 */
			public readFloat(position: bigint): number;

			/**
			 * Read a 64-bit floating-point number from the data stream (with or without position)
			 *
			 * This method reads eight bytes from the data stream and interprets them as a double-precision (64-bit) floating-point number (double).
			 * The byte order (big-endian or little-endian) might depend on the Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by 8 bytes.
			 *
			 * @returns {number} The 64-bit floating-point number value read from the data stream.
			 */
			public readDouble(): number; // This should return a number, not bigint

			/**
			 * Read a 64-bit floating-point number from the data stream at a specific position
			 *
			 * Similar to readDouble, this method reads eight bytes from the data stream and interprets them as a double.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {number} The 64-bit floating-point number value read from the data stream.
			 */
			public readDouble(position: bigint): number;

			/**
			 * Read a zero-terminated string from the data stream (with or without position)
			 *
			 * This method reads bytes from the data stream until a null terminator (byte with value 0) is encountered.
			 * The bytes (excluding the null terminator) are interpreted as a UTF-8 encoded string.
			 * If a size is specified, it indicates the maximum number of bytes to read, including the null terminator.
			 * If no size is specified, all bytes up to the next null terminator are read.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position.
			 * The read position is advanced by the number of bytes read, including the null terminator.
			 *
			 * @param {bigint} size - The maximum number of bytes to read (including null terminator). Optional.
			 * @returns {string} The zero-terminated string value read from the data stream.
			 */
			public readString(size: bigint): string;

			/**
			 * Read a zero-terminated string from the data stream at a specific position with a specified size
			 *
			 * Similar to readString, this method reads bytes from a specified position up to the null terminator or the specified size (whichever comes first).
			 * However, it requires both a position and size argument.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} size - The maximum number of bytes to read (including null terminator).
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The zero-terminated string value read from the data stream.
			 */
			public readString(size: bigint, position: bigint): string;

			/**
			 * Read a string from the data stream by reading a specified number of bytes and interpreting them as UTF-8 encoded string
			 *
			 * This method reads the specified size (in bytes) from the data stream and interprets them as a UTF-8 encoded string.
			 * The null terminator is not expected or checked in this method.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by the size argument.
			 *
			 * @param {bigint} size - The number of bytes to read from the data stream.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByUint8(): string;

			/**
			 * Read a string from the data stream by reading a specified number of bytes and interpreting them as UTF-8 encoded string at a specific position
			 *
			 * Similar to readStringByUint8, this method reads the specified size (in bytes) from the data stream at the given position.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} size - The number of bytes to read from the data stream.
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByUint8(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 16-bit unsigned integer and interpreting them as UTF-8 encoded string
			 *
			 * This method first reads two bytes from the data stream and interprets them as an unsigned 16-bit integer (uint16).
			 * This uint16 value represents the number of bytes to read for the string data.
			 * Then, it reads that many bytes from the data stream and interprets them as a UTF-8 encoded string.
			 * The null terminator is not expected or checked in this method.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by the size determined from uint16 + 2 (for the uint16 value itself).
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByUint16(): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 16-bit unsigned integer and interpreting them as UTF-8 encoded string at a specific position
			 *
			 * Similar to readStringByUint16, this method reads a uint16 value first, then reads that many bytes for the string data.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByUint16(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 24-bit unsigned integer and interpreting them as UTF-8 encoded string (less common)
			 *
			 * This method is similar to readStringByUint16 but uses a 24-bit unsigned integer (uint24) to determine the string length.
			 * Less common than other string reading methods, check if this method is supported by the specific Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by the size determined from uint24 + 3 (for the uint24 value itself).
			 *
			 * @returns {string} The string value read from the data stream (if supported), interpreted as UTF-8 encoded.
			 */
			public readStringByUint24(): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 24-bit unsigned integer and interpreting them as UTF-8 encoded string (less common)
			 *
			 * Similar to readStringByUint16, this method is similar but uses a 24-bit unsigned integer (uint24) to determine the string length.
			 * Less common than other string reading methods, check if this method is supported by the specific Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by the size determined from uint24 + 3 (for the uint24 value itself).
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream (if supported), interpreted as UTF-8 encoded.
			 */
			public readStringByUint24(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 32-bit unsigned integer and interpreting them as UTF-8 encoded string
			 *
			 * This method first reads four bytes from the data stream and interprets them as an unsigned 32-bit integer (uint32).
			 * This uint32 value represents the number of bytes to read for the string data.
			 * Then, it reads that many bytes from the data stream and interprets them as a UTF-8 encoded string.
			 * The null terminator is not expected or checked in this method.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by the size determined from uint32 + 4 (for the uint32 value itself).
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByUint32(): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 32-bit unsigned integer and interpreting them as UTF-8 encoded string at a specific position
			 *
			 * Similar to readStringByUint32, this method reads a uint32 value first, then reads that many bytes for the string data.
			 * However, it requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByUint32(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 64-bit unsigned integer and interpreting them as UTF-8 encoded string (least common)
			 *
			 * This method is similar to readStringByUint32 but uses a 64-bit unsigned integer (uint64) to determine the string length.
			 * Less common than other string reading methods, check if this method is supported by the specific Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position and the read position is advanced by the size determined from uint64 + 8 (for the uint64 value itself).
			 *
			 * @returns {string} The string value read from the data stream (if supported), interpreted as UTF-8 encoded.
			 */
			public readStringByUint64(): string;

			/**
			 * Read a string from the data stream by reading a number of bytes determined by a 64-bit unsigned integer and interpreting them as UTF-8 encoded string at a specific position (least common)
			 *
			 * Similar to readStringByUint64, this method uses uint64 to determine the string length at a specified position.
			 * Less common than other string reading methods, check if this code is supported by the specific Kernel framework.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream (if supported), interpreted as UTF-8 encoded.
			 */
			public readStringByUint64(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a signed 8-bit integer (int8) as the size and interpreting the following bytes as UTF-8 encoded string (not recommended)
			 *
			 * This method is not recommended as it uses a signed integer (int8) for the string length, which can only represent a limited range of values (from -128 to 127).
			 * It's better to use methods with unsigned integers (uint8, uint16, etc.) for string lengths.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the size (int8 value) + 1 (for the int8 value itself).
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt8(): string;

			/**
			 * Read a string from the data stream by reading a signed 8-bit integer (int8) as the size and interpreting the following bytes as UTF-8 encoded string at a specific position (not recommended)
			 *
			 * Similar to readStringByInt8, this method is not recommended as it uses a signed integer for the string length.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt8(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a signed 16-bit integer (int16) as the size and interpreting the following bytes as UTF-8 encoded string (not recommended)
			 *
			 * This method is similar to readStringByInt8 but uses a signed 16-bit integer (int16) for the string length.
			 * Similar to int8, int16 also has limitations in representing string lengths. It's better to use methods with unsigned integers.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the size (int16 value) + 2 (for the int16 value itself).
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt16(): string;

			/**
			 * Read a string from the data stream by reading a signed 16-bit integer (int16) as the size and interpreting the following bytes as UTF-8 encoded string at a specific position (not recommended)
			 *
			 * Similar to readStringByInt16, this method is not recommended due to limitations of int16 for string lengths.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt16(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a signed 24-bit integer (int24) as the size and interpreting the following bytes as UTF-8 encoded string (not recommended)
			 *
			 * This method is similar to readStringByInt8 and readStringByInt16, but uses a signed 24-bit integer (int24) for the string length.
			 * Similar to int8 and int16, int24 also has limitations in representing string lengths. It's better to use methods with unsigned integers.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the size (int24 value) + 3 (for the int24 value itself).
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt24(): string;

			/**
			 * Read a string from the data stream by reading a signed 24-bit integer (int24) as the size and interpreting the following bytes as UTF-8 encoded string at a specific position (not recommended)
			 *
			 * Similar to readStringByInt24, this method is not recommended due to limitations of int24 for string lengths.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt24(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a signed 32-bit integer (int32) as the size and interpreting the following bytes as UTF-8 encoded string (not recommended)
			 *
			 * This method is similar to previous methods using signed integers for string length, but uses a signed 32-bit integer (int32).
			 * While int32 offers a wider range than int8, int16, and int24, it's still generally recommended to use unsigned integers for string lengths.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the size (int32 value) + 4 (for the int32 value itself).
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt32(): string;

			/**
			 * Read a string from the data stream by reading a signed 32-bit integer (int32) as the size and interpreting the following bytes as UTF-8 encoded string at a specific position (not recommended)
			 *
			 * Similar to readStringByInt32, this method is not recommended due to limitations of int32 for string lengths.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByInt32(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a variable-length encoded unsigned 32-bit integer (varInt32) as the size and interpreting the following bytes as UTF-8 encoded string
			 *
			 * This method reads a varInt32-encoded integer, which is a compact way to store numbers that frequently have small values.
			 * The varInt32 encoding uses one or more bytes, where the first byte stores the sign and number of higher-order bytes used.
			 * Then, it reads that many bytes following the first byte and interprets them (with the sign from the first byte) as the size of the string data.
			 * Finally, it reads that size in bytes from the data stream and interprets them as UTF-8 encoded string.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the total number of bytes used for the varInt32 encoding and the size of the string data.
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByVarInt32(): string;

			/**
			 * Read a string from the data stream by reading a variable-length encoded unsigned 32-bit integer (varInt32) as the size and interpreting the following bytes as UTF-8 encoded string at a specific position
			 *
			 * Similar to readStringByVarInt32, this method uses varInt32 encoding to determine the string length but at a specified position.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded.
			 */
			public readStringByVarInt32(position: bigint): string;

			/**
			 * Read a string from the data stream until a null terminator is encountered (not recommended)
			 *
			 * This method is not recommended because it relies on the presence of a null terminator (byte with value 0) to determine the string length.
			 * This can be error-prone if the data stream does not contain a null terminator at the expected location.
			 * It's generally better to use methods with a specified size for the string data.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the number of bytes read until the null terminator is found (excluding the null terminator itself).
			 *
			 * @returns {string} The string value read from the data stream, interpreted as UTF-8 encoded (until null terminator is found).
			 */
			public readStringByEmpty(): string;

			/**
			 * Read a string from the data stream until a null terminator is encountered at a specific position (not recommended)
			 *
			 * Similar to readStringByEmpty, this method is not recommended due to reliance on a null terminator.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream (until null terminator is found), interpreted as UTF-8 encoded.
			 */
			public readStringByEmpty(position: bigint): string;

			/**
			 * Read a string from the data stream by reading a signed 64-bit integer (int64) as the size and interpreting the following bytes as UTF-8 encoded string (least common)
			 *
			 * This method is similar to readStringByUint64 but uses a signed 64-bit integer (int64) to determine the string length.
			 * Less common than other string reading methods, check if this method is supported by the specific Kernel framework.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the size (int64 value) + 8 (for the int64 value itself).
			 *
			 * @returns {string} The string value read from the data stream (if supported), interpreted as UTF-8 encoded.
			 */
			public readStringByInt64(): string;

			/**
			 * Read a string from the data stream by reading a signed 64-bit integer (int64) as the size and interpreting the following bytes as UTF-8 encoded string at a specific position (least common)
			 *
			 * Similar to readStringByInt64, this method uses int64 to determine the string length at a specified position.
			 * Less common than other string reading methods, check if this code is supported by the specific Kernel framework.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {string} The string value read from the data stream (if supported), interpreted as UTF-8 encoded.
			 */
			public readStringByInt64(position: bigint): string;

			/**
			 * Read a variable-length encoded unsigned 32-bit integer (varInt32) from the data stream
			 *
			 * This method reads a varInt32-encoded integer, which is a compact way to store numbers that frequently have small values.
			 * The varInt32 encoding uses one or more bytes, where the first byte stores the sign and number of higher-order bytes used.
			 * Then, it interprets the following bytes (based on the first byte) as an unsigned 32-bit integer.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the total number of bytes used for the varInt32 encoding.
			 *
			 * @returns {bigint} The unsigned 32-bit integer value read from the data stream.
			 */
			public readVarInt32(): bigint;

			/**
			 * Read a variable-length encoded unsigned 32-bit integer (varInt32) from the data stream at a specific position
			 *
			 * Similar to readVarInt32, this method reads a varInt32 encoded integer but at a specified position.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 32-bit integer value read from the data stream.
			 */
			public readVarInt32(position: bigint): bigint;

			/**
			 * Read a variable-length encoded unsigned 64-bit integer (varInt64) from the data stream
			 *
			 * This method is similar to readVarInt32 but reads a varInt64-encoded integer, which is a compact way to store larger unsigned integers.
			 * The varInt64 encoding uses one or more bytes, similar to varInt32, to represent the unsigned 64-bit integer.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the total number of bytes used for the varInt64 encoding.
			 *
			 * @returns {bigint} The unsigned 64-bit integer value read from the data stream.
			 */
			public readVarInt64(): bigint;

			/**
			 * Read a variable-length encoded unsigned 64-bit integer (varInt64) from the data stream at a specific position
			 *
			 * Similar to readVarInt64, this method reads a varInt64 encoded integer but at a specified position.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 64-bit integer value read from the data stream.
			 */
			public readVarInt64(position: bigint): bigint;

			/**
			 * Read a variable-length encoded unsigned 32-bit integer (varInt32) from the data stream (alias for readVarInt32)
			 *
			 * This method is an alias for readVarInt32. It reads a varInt32-encoded unsigned 32-bit integer.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the total number of bytes used for the varInt32 encoding.
			 *
			 * @returns {bigint} The unsigned 32-bit integer value read from the data stream.
			 */
			public readVarUint32(): bigint;

			/**
			 * Read a variable-length encoded unsigned 32-bit integer (varInt32) from the data stream at a specific position (alias for readVarInt32)
			 *
			 * Similar to readVarUint32 (alias for readVarInt32), this method reads a varInt32 encoded integer but at a specified position.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 32-bit integer value read from the data stream.
			 */
			public readVarUint32(position: bigint): bigint;

			/**
			 * Read a variable-length encoded unsigned 64-bit integer (varInt64) from the data stream (alias for readVarInt64)
			 *
			 * This method is an alias for readVarInt64. It reads a varInt64-encoded unsigned 64-bit integer.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by the total number of bytes used for the varInt64 encoding.
			 *
			 * @returns {bigint} The unsigned 64-bit integer value read from the data stream.
			 */
			public readVarUint64(): bigint;

			/**
			 * Read a variable-length encoded unsigned 64-bit integer (varInt64) from the data stream at a specific position (alias for readVarInt64)
			 *
			 * Similar to readVarUint64 (alias for readVarInt64), this method reads a varInt64 encoded integer but at a specified position.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The unsigned 64-bit integer value read from the data stream.
			 */
			public readVarUint64(position: bigint): bigint;

			/**
			 * Read a signed 32-bit integer encoded in zig-zag format from the data stream
			 *
			 * This method reads a 32-bit integer encoded in zig-zag format, which is a technique to efficiently encode signed integers using unsigned integer storage.
			 * The encoding process involves alternating the sign bit with the actual value bits while shifting right by one.
			 * Decoding involves reversing this process.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by 4 bytes (for the 32-bit integer).
			 *
			 * @returns {bigint} The signed 32-bit integer value read from the data stream, decoded from zig-zag format.
			 */
			public readZigZag32(): bigint;

			/**
			 * Read a signed 32-bit integer encoded in zig-zag format from the data stream at a specific position
			 *
			 * Similar to readZigZag32, this method reads a zig-zag encoded signed integer but at a specified position.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The signed 32-bit integer value read from the data stream (decoded from zig-zag format).
			 */
			public readZigZag32(position: bigint): bigint;

			/**
			 * Read a signed 64-bit integer encoded in zig-zag format from the data stream
			 *
			 * Similar to readZigZag32, this method reads a 64-bit integer encoded in zig-zag format.
			 * If a position is specified, it indicates the byte offset from where the value should be read.
			 * Otherwise, the value is read from the current read position. The read position is advanced by 8 bytes (for the 64-bit integer).
			 *
			 * @returns {bigint} The signed 64-bit integer value read from the data stream, decoded from zig-zag format.
			 */
			public readZigZag64(): bigint;

			/**
			 * Read a signed 64-bit integer encoded in zig-zag format from the data stream at a specific position
			 *
			 * Similar to readZigZag64, this method reads a zig-zag encoded signed integer but at a specified position.
			 * It requires a position argument specifying the exact byte offset to read from.
			 * The read position is not advanced in this case.
			 *
			 * @param {bigint} position - The byte offset from where the value should be read.
			 * @returns {bigint} The signed 64-bit integer value read from the data stream (decoded from zig-zag format).
			 */
			public readZigZag64(position: bigint): bigint;

			/**
			 * Close the current data stream
			 *
			 * This method closes the underlying data stream associated with the DataStreamView object.
			 * It's important to call this method when you're finished using the stream to release resources.
			 *
			 * @returns {void} (doesn't return a value)
			 */
			public close(): void;
		}

		/**
		 * Defines the brush type used for drawing on the canvas.
		 *
		 * * 0n: Represents the fill style, used to fill shapes.
		 * * 1n: Represents the stroke style, used to outline shapes.
		 */
		export type BrushType = 0n | 1n;

		/**
		 * This class represents a canvas element for drawing operations.
		 * It provides functionalities like creating a canvas, scaling it, and potentially more for advanced drawing.
		 */

		declare class Canvas {
			// Self declaration for memory usage

			private _Canvas: Canvas;

			/**
			 * Creates a new canvas instance with the specified width and height.
			 *
			 * @param width - The width of the canvas in bigint.
			 * @param height - The height of the canvas in bigint.
			 */
			public constructor(width: bigint, height: bigint): void;

			/**
			 * Scales the canvas by the specified x and y factors.
			 *
			 * This allows you to zoom in or out on the drawing content.
			 *
			 * @param x - The scale factor for the x-axis (positive number).
			 * @param y - The scale factor for the y-axis (positive number).
			 */
			public scale(x: number, y: number): void;

			/**
			 * Rotates the context by a specified angle, in radians.
			 * This effectively rotates all subsequent drawing operations.
			 *
			 * @param angle - The angle of rotation in radians.
			 */
			public rotate(angle: number): void;

			/**
			 * Shifts the drawing context to a new position on the canvas.
			 * This affects where subsequent drawing operations will be placed.
			 *
			 * @param x - The new x-coordinate to move the context to.
			 * @param y - The new y-coordinate to move the context to.
			 */
			public translate(x: number, y: number): void;

			/**
			 * Applies a custom transformation matrix to the context, allowing for advanced
			 * manipulations of shapes and drawings. This can be used for scaling, skewing,
			 * rotating, and translating in a single operation.
			 *
			 * @param a - Horizontal scaling factor.
			 * @param b - Horizontal skewing factor.
			 * @param c - Vertical skewing factor.
			 * @param d - Vertical scaling factor.
			 * @param e - Horizontal translation factor.
			 * @param f - Vertical translation factor.
			 */
			public transform(
				a: number,
				b: number,
				c: number,
				d: number,
				e: number,
				f: number,
			): void;

			/**
			 * Resets the current transformation to the identity matrix (no transformations),
			 * and then applies a new transformation matrix, effectively replacing any
			 * previous transformations with the specified one.
			 *
			 * @param a - Horizontal scaling factor.
			 * @param b - Horizontal skewing factor.
			 * @param c - Vertical skewing factor.
			 * @param d - Vertical scaling factor.
			 * @param e - Horizontal translation factor.
			 * @param f - Vertical translation factor.
			 */
			public set_transform(
				a: number,
				b: number,
				c: number,
				d: number,
				e: number,
				f: number,
			): void;

			/**
			 * Sets the overall transparency (alpha) value for all subsequent drawings on the canvas.
			 *
			 * @param alpha - The alpha value, ranging from 0.0 (fully transparent) to 1.0 (fully opaque).
			 */
			public set_global_alpha(alpha: number): void;

			/**
			 * Sets the color used for shadows in drawings.
			 *
			 * @param a - Red component of the shadow color (0-255).
			 * @param b - Green component of the shadow color (0-255).
			 * @param c - Blue component of the shadow color (0-255).
			 * @param d - Alpha component of the shadow color (0-255).
			 */
			public set_shadow_color(a: number, b: number, c: number, d: number): void;

			/**
			 * Sets the blur level for shadows in drawings. Higher values create softer shadows.
			 *
			 * @param blur - The blur level (positive number).
			 */
			public set_shadow_blur(blur: number): void;

			/**
			 * Sets the width of lines used for drawing shapes and strokes.
			 *
			 * @param width - The line width (positive number).
			 */
			public set_line_width(width: number): void;

			/**
			 * Sets the miter limit ratio, which affects how sharp corners of paths are drawn.
			 * A higher limit allows for sharper corners before miters are beveled.
			 *
			 * @param limit - The miter limit ratio (positive number).
			 */
			public set_miter_limit(limit: number): void;

			/**
			 * Sets the drawing color for the specified brush type.
			 *
			 * @param type - The type of brush to apply the color to (0n for fill, 1n for stroke).
			 * @param red - Red component of the color (0-1).
			 * @param green - Green component of the color (0-1).
			 * @param blue - Blue component of the color (0-1).
			 * @param alpha - Alpha component of the color (0.0 for fully transparent, 1.0 for fully opaque).
			 */
			public set_color(
				type: BrushType,
				red: number,
				green: number,
				blue: number,
				alpha: number,
			): void;

			/**
			 * Creates a linear gradient brush for the specified brush type.
			 * This brush can be used for creating smooth color transitions within shapes.
			 *
			 * @param type - The type of brush to create (0n for fill, 1n for stroke).
			 * @param start_x - The x-coordinate of the gradient's starting point.
			 * @param start_y - The y-coordinate of the gradient's starting point.
			 * @param end_x - The x-coordinate of the gradient's ending point.
			 * @param end_y - The y-coordinate of the gradient's ending point.
			 */
			public set_linear_gradient(
				type: BrushType,
				start_x: number,
				start_y: number,
				end_x: number,
				end_y: number,
			): void;

			/**
			 * Creates a radial gradient brush.
			 * @param type The type of brush to create.
			 * @param start_x The x-coordinate of the start circle.
			 * @param start_y The y-coordinate of the start circle.
			 * @param start_radius The radius of the start circle.
			 * @param end_x The x-coordinate of the end circle.
			 * @param end_y The y-coordinate of the end circle.
			 * @param end_radius The radius of the end circle.
			 */
			public set_radial_gradient(
				type: BrushType,
				start_x: number,
				start_y: number,
				start_radius: number,
				end_x: number,
				end_y: number,
				end_radius: number,
			): void;

			/**
			 * Begins a new path by emptying the list of sub-paths. Call this method when you want to create a new path.
			 */
			public begin_path(): void;

			/**
			 * Closes the current path.
			 */
			public close_path(): void;

			/**
			 * Moves the pen to a specified point in the canvas, without creating a line.
			 * @param x The x-coordinate of the point.
			 * @param y The y-coordinate of the point.
			 */
			public move_to(x: number, y: number): void;

			/**
			 * Connects the last point in the current path to the specified point with a straight line.
			 * @param x The x-coordinate of the point.
			 * @param y The y-coordinate of the point.
			 */
			public line_to(x: number, y: number): void;

			/**
			 * Adds a quadratic Bézier curve to the current path.
			 * @param control_x The x-coordinate of the control point.
			 * @param control_y The y-coordinate of the control point.
			 * @param x The x-coordinate of the end point.
			 * @param y The y-coordinate of the end point.
			 */
			public quadratic_curve_to(
				control_x: number,
				control_y: number,
				x: number,
				y: number,
			): void;

			/**
			 * Adds a cubic Bézier curve to the current path.
			 * @param control_1_x The x-coordinate of the first control point.
			 * @param control_1_y The y-coordinate of the first control point.
			 * @param control_2_x The x-coordinate of the second control point.
			 * @param control_2_y The y-coordinate of the second control point.
			 * @param x The x-coordinate of the end point.
			 * @param y The y-coordinate of the end point.
			 */
			public bezier_curve_to(
				control_1_x: number,
				control_1_y: number,
				control_2_x: number,
				control_2_y: number,
				x: number,
				y: number,
			): void;

			/**
			 * Creates a path to a point and creates an arc.
			 * @param vertex_x The x-coordinate of the tangent point of the arc.
			 * @param vertex_y The y-coordinate of the tangent point of the arc.
			 * @param x The x-coordinate of the end point of the arc.
			 * @param y The y-coordinate of the end point of the arc.
			 * @param radius The radius of the arc.
			 */
			public arc_to(
				vertex_x: number,
				vertex_y: number,
				x: number,
				y: number,
				radius: number,
			): void;

			/**
			 * Draws an arc/curve.
			 * @param x The x-coordinate of the center of the circle.
			 * @param y The y-coordinate of the center of the circle.
			 * @param radius The radius of the circle.
			 * @param start_angle The starting angle, in radians (0 is at the 3 o'clock position of the arc's circle).
			 * @param end_angle The ending angle, in radians.
			 * @param counter_clockwise Optional. Specifies whether the drawing should be counterclockwise or clockwise. False is default, and indicates clockwise.
			 */
			public arc(
				x: number,
				y: number,
				radius: number,
				start_angle: number,
				end_angle: number,
				counter_clockwise: boolean,
			): void;

			/**
			 * Draws a rectangle.
			 * @param x The x-coordinate of the rectangle's starting point.
			 * @param y The y-coordinate of the rectangle's starting point.
			 * @param width The rectangle's width.
			 * @param height The rectangle's height.
			 */
			public rectangle(x: number, y: number, width: number, height: number): void;

			/**
			 * Fills the current or given path with the current fill style.
			 */
			public fill(): void;

			/**
			 * Strokes (outlines) the current or given path with the current stroke style.
			 */
			public stroke(): void;

			/**
			 * Turns the path currently being built into the current clipping path.
			 */
			public clip(): void;

			/**
			 * Reports whether the specified point is contained in the current path.
			 * @param x The x-coordinate of the point to check.
			 * @param y The y-coordinate of the point to check.
			 * @return Boolean indicating whether the point is in the path.
			 */
			public is_point_in_path(x: number, y: number): boolean;

			/**
			 * Clears the specified rectangular area, making it fully transparent.
			 * @param x The x-coordinate of the rectangle's starting point.
			 * @param y The y-coordinate of the rectangle's starting point.
			 * @param width The rectangle's width.
			 * @param height The rectangle's height.
			 */
			public clear_rectangle(x: number, y: number, width: number, height: number): void;

			/**
			 * Draws a filled rectangle at the specified coordinates.
			 * @param x The x-coordinate of the rectangle's starting point.
			 * @param y The y-coordinate of the rectangle's starting point.
			 * @param width The rectangle's width.
			 * @param height The rectangle's height.
			 */
			public fill_rectangle(x: number, y: number, width: number, height: number): void;

			/**
			 * Draws a rectangle that is stroked (outlined) according to the current strokeStyle and other context settings.
			 * @param x The x-coordinate of the rectangle's starting point.
			 * @param y The y-coordinate of the rectangle's starting point.
			 * @param width The rectangle's width.
			 * @param height The rectangle's height.
			 */
			public stroke_rectangle(x: number, y: number, width: number, height: number): void;

			/**
			 * Sets the font to be used for text operations.
			 * @param font The font data in ArrayBuffer format.
			 * @param bytes The size of the font data in bytes.
			 * @return Boolean indicating whether the font was successfully set.
			 */
			public set_font(font: ArrayBuffer, bytes: bigint): boolean;

			/**
			 * Draws an image onto the canvas.
			 * @param image The image data in ArrayBuffer format.
			 * @param width The width of the source image.
			 * @param height The height of the source image.
			 * @param stride The number of bytes per row in the source image.
			 * @param x The x-coordinate on the canvas at which to place the top-left corner of the source image.
			 * @param y The y-coordinate on the canvas at which to place the top-left corner of the source image.
			 * @param to_width The width to draw the image in the canvas.
			 * @param to_height The height to draw the image in the canvas.
			 */
			public draw_image(
				image: ArrayBuffer,
				width: bigint,
				height: bigint,
				stride: bigint,
				x: number,
				y: number,
				to_width: number,
				to_height: number,
			): void;

			/**
			 * Gets the image data from the specified rectangle.
			 * @param image The ArrayBuffer where the image data will be stored.
			 * @param width The width of the rectangle from which the image data will be extracted.
			 * @param height The height of the rectangle from which the image data will be extracted.
			 * @param stride The number of bytes per row in the output image data.
			 * @param x The x-coordinate of the upper-left corner of the rectangle from which the image data will be extracted.
			 * @param y The y-coordinate of the upper-left corner of the rectangle from which the image data will be extracted.
			 */
			public get_image_data(
				image: ArrayBuffer,
				width: bigint,
				height: bigint,
				stride: bigint,
				x: number,
				y: number,
			): void;

			/**
			 * Puts the image data into a specified rectangle of the canvas.
			 * @param image The image data in ArrayBuffer format to be placed on the canvas.
			 * @param width The width of the image data.
			 * @param height The height of the image data.
			 * @param stride The number of bytes per row in the image data.
			 * @param x The x-coordinate of the upper-left corner of the rectangle where the image data will be placed.
			 * @param y The y-coordinate of the upper-left corner of the rectangle where the image data will be placed.
			 */
			public put_image_data(
				image: ArrayBuffer,
				width: bigint,
				height: bigint,
				stride: bigint,
				x: bigint,
				y: bigint,
			): void;

			/**
			 * Saves the entire state of the canvas by pushing the current state onto a stack.
			 */
			public save(): void;

			/**
			 * Restores the most recently saved canvas state by popping the top entry off the stack.
			 */
			public restore(): void;

			/**
			 * Set the color of the image
			 */
			public set_image_color(red: number, green: number, blue: number, alpha: number): void;
		}

		/**
		 * This namespace provides a collection of miscellaneous utility functions for various tasks.
		 */

		declare namespace Miscellaneous {
			/**
			 * Creates a deep copy of a JavaScript value, including nested objects and arrays.
			 * This is useful for creating independent copies of objects to avoid unintended modifications.
			 *
			 * @param object - The object to be cloned. While it can be any JavaScript value, it's typically used for objects.
			 * @returns A new, independent copy of the provided object.
			 */
			export function deep_copy<T extends any>(object: T): T;

			/**
			 * to_apng function
			 *
			 * This function generates an Animated Portable Network Graphic (APNG) image from a list of image paths.
			 *
			 * @param image_path_list  An array of strings containing the paths to the individual image frames for the animation.
			 * @param destination     The destination path (including filename) where the generated APNG image will be saved.
			 * @param setting          A Setting object containing configuration options for the APNG generation process.
			 *                          This object likely controls aspects like quality, disposal method, and blending mode.
			 * @throws                 An error if there are issues processing the images or saving the APNG file.
			 */
			export function to_apng(
				image_path_list: Array<string>,
				destination: string,
				setting: Kernel.Encoding.APNG.Setting,
			): void;
		}
	}
}
