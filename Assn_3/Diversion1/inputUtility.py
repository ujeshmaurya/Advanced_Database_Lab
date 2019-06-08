# Utility functions for parsing input

# This function extracts the transaction id from the token
def get_transaction_id(token):
	ind = token.find('(')
	return int(token[2 : ind])

# This function extracts the operation type from the token
def get_operation_type(token):
	if token[0] == 'r' or token[0] == 'R':
		return "read"
	return "write"

# This function extracts the data item (on which operation is performed) from the token
def get_data_item(token):
	ind1 = token.find('(')
	ind2 = token.find(')')
	return token[ind1 + 1 : ind2]

# This function extracts the number of transactions in a given schedule
def get_num_of_transactions(schedule):
	num = 0
	schedule_list = schedule.split()
	for token in schedule_list:
		num = max(num, int(token[2 : token.find('(')]))
	return num