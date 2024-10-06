# Encryption:
# If the target is a directory, it will first be compressed into a .tar.gz archive.
# The archive or file is then encrypted using openssl with aes-256-cbc, -salt, and -pbkdf2 options.
# The encrypted output will default to <file>.enc if no OUTPUT_FILE is provided.
# 
# Decryption:
# If the target is an encrypted file or directory (with .enc extension), it decrypts it using openssl.
# If the decrypted content is a .tar.gz archive (i.e., it was a directory), it will be extracted to its original form.
# The output file defaults to the original name (without .enc) unless OUTPUT_FILE is provided.
# 
# Help:
# The help target provides a summary of how to use the Makefile for encryption and decryption.
# 
# Parameters:
# 
#     FILE: The file or directory to encrypt/decrypt (required).
#     OUTPUT_FILE: The output file or directory (optional).
# 
# Example Usage:
# 
# Encrypt a File:
# 
# $ make encrypt FILE=./file.txt OUTPUT_FILE=./encrypted_file.enc
# 
# Encrypt a Directory:
# 
# $ make encrypt FILE=./my_directory OUTPUT_FILE=./encrypted_directory.enc
# 
# Decrypt a File:
# 
# $ make decrypt FILE=./encrypted_file.enc OUTPUT_FILE=./decrypted_file.txt
# 
# Decrypt a Directory:
# 
# $ make decrypt FILE=./encrypted_directory.enc OUTPUT_FILE=./decrypted_directory
# 
# This solution keeps everything self-contained in the Makefile, eliminating the need for an external script. Let me know if you need more features or modifications!
# 

# Default encryption parameters
ENCRYPTION_ALGO := aes-256-cbc
ENCRYPTION_SALT := -salt
ENCRYPTION_PBKDF2 := -pbkdf2

# Encrypt a file or directory
encrypt:
	@if [ -z "$(FILE)" ]; then \
		echo "Error: You must specify a file or directory to encrypt using FILE=<file>"; \
		exit 1; \
	fi
	@if [ -z "$(OUTPUT_FILE)" ]; then \
		OUTPUT_FILE="$(FILE).enc"; \
	fi
	@read -sp "Enter password: " PASSWORD; echo; \
	if [ -d "$(FILE)" ]; then \
		echo "Encrypting directory..."; \
		tar -czf "$(FILE).tar.gz" -C "$(dir $(FILE))" "$(notdir $(FILE))"; \
		openssl enc $(ENCRYPTION_ALGO) $(ENCRYPTION_SALT) $(ENCRYPTION_PBKDF2) -in "$(FILE).tar.gz" -out "$(OUTPUT_FILE)" -pass pass:$$PASSWORD; \
		rm "$(FILE).tar.gz"; \
	else \
		echo "Encrypting file..."; \
		openssl enc $(ENCRYPTION_ALGO) $(ENCRYPTION_SALT) $(ENCRYPTION_PBKDF2) -in "$(FILE)" -out "$(OUTPUT_FILE)" -pass pass:$$PASSWORD; \
	fi
	@echo "Encryption completed: $(OUTPUT_FILE)"

# Decrypt a file or directory
decrypt:
	@if [ -z "$(FILE)" ]; then \
		echo "Error: You must specify a file or directory to decrypt using FILE=<file>"; \
		exit 1; \
	fi
	@if [ -z "$(OUTPUT_FILE)" ]; then \
		OUTPUT_FILE="$(FILE%.enc)"; \
	fi
	@read -sp "Enter password: " PASSWORD; echo; \
	if [ "$(FILE)" != "$(FILE%.enc)" ]; then \
		if [ -d "$(OUTPUT_FILE)" ]; then \
			echo "Decrypting directory..."; \
			openssl enc -d $(ENCRYPTION_ALGO) $(ENCRYPTION_PBKDF2) -in "$(FILE)" -out "$(OUTPUT_FILE).tar.gz" -pass pass:$$PASSWORD; \
			tar -xzf "$(OUTPUT_FILE).tar.gz" -C "$(OUTPUT_FILE)"; \
			rm "$(OUTPUT_FILE).tar.gz"; \
		else \
			echo "Decrypting file..."; \
			openssl enc -d $(ENCRYPTION_ALGO) $(ENCRYPTION_PBKDF2) -in "$(FILE)" -out "$(OUTPUT_FILE)" -pass pass:$$PASSWORD; \
		fi; \
	else \
		echo "Invalid encrypted file. Expected a '.enc' extension."; \
		exit 1; \
	fi
	@echo "Decryption completed: $(OUTPUT_FILE)"
