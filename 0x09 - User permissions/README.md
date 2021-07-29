# User permission

Current authenticated user identity can be retrieved through `id` & `groups` commands.  
On Linux, all identity related information, is spread accross several files.

## Users

User database is accessible through `/etc/passwd`, it is structured this way:  
* **Username**: It is used when user logs in. It should be between 1 and 32 characters in length.
* **Password**: An x character indicates that encrypted password is stored in /etc/shadow file. Please note that you need to use the passwd command to computes the hash of a password typed at the CLI or to store/update the hash of the password in `/etc/shadow` file.
* **User ID (UID)**: Each user must be assigned a user ID (UID). UID 0 (zero) is reserved for root and UIDs 1-99 are reserved for other predefined accounts. Further UID 100-999 are reserved by system for administrative and system accounts/groups.
* **Group ID (GID)**: The primary group ID (stored in `/etc/group` file)
* **User ID Info**: The comment field. It allow you to add extra information about the users such as user’s full name, phone number etc. This field use by finger command.
* **Home directory**: The absolute path to the directory the user will be in when they log in. If this directory does not exists then users directory becomes /
* **Command/shell**: The absolute path of a command or shell (/bin/bash). Typically, this is a shell. Please note that it does not have to be a shell.

_Source: https://www.cyberciti.biz/faq/understanding-etcpasswd-file-format/_

## Passwords

Passwords are stored in `/etc/shadow`, it's structured this way:  
* **Username** : It is your login name.
* **Password** : It is your encrypted password. The password should be minimum 8-12 characters long including special characters, digits, lower case alphabetic and more. Usually password format is set to $id$salt$hashed, The $id is the algorithm used On GNU/Linux as follows:
  * $1$ is MD5
  * $2a$ is Blowfish
  * $2y$ is Blowfish
  * $5$ is SHA-256
  * $6$ is SHA-512
* **Last password change (lastchanged)** : Days since Jan 1, 1970 that password was last changed
* **Minimum** : The minimum number of days required between password changes i.e. the number of days left before the user is allowed to change his/her password
* **Maximum** : The maximum number of days the password is valid (after that user is forced to change his/her password)
* **Warn** : The number of days before password is to expire that user is warned that his/her password must be changed
* **Inactive** : The number of days after password expires that account is disabled
* **Expire** : days since Jan 1, 1970 that account is disabled i.e. an absolute date specifying when the login may no longer be used.

_Source: https://www.cyberciti.biz/faq/understanding-etcshadow-file/_

## Groups

Groups are stored in `/etc/groups`, it's sturctured this way:  
* **group_name**: It is the name of group. If you run ls -l command, you will see this name printed in the group field.
* **Password**: Generally password is not used, hence it is empty/blank. It can store encrypted password. This is useful to implement privileged groups.
* **Group ID (GID)**: Each user must be assigned a group ID. You can see this number in your /etc/passwd file.
* **Group List**: It is a list of user names of users who are members of the group. The user names, must be separated by commas.

_Source: https://www.cyberciti.biz/faq/understanding-etcgroup-file/_

## File permissions

When using `ls -l` we can have the detailed informations about all files contained in the current/working directory.

Among those information, we can see the permissions. The first segment of the output represent the required permissions to access the file.

`-rwxrwxrwx` can be decomposed in three triads:
* Owner assigned permission (2-4)
* Group assigned permission (5-7)
* Other assigned permission (8-10)

For each triad, permissions are described as follow:

| Character | Effect on files              | Effect on directories                                                                      |
|-----------|------------------------------|--------------------------------------------------------------------------------------------|
| <td colspan=2>**Read permission**                                                                                                     |
| -	        | The file cannot be read.     | The directory's contents cannot be shown.                                                  |
| r         | The file can be read.        | The directory's contents can be shown.                                                     |
| <td colspan=2>**Write permission**                                                                                                    |
| -	        | The file cannot be modified. | The directory's contents cannot be modified.                                               |
| w         | The file can be modified.    | The directory's contents can be modified.                                                  |
| <td colspan=2>**Execute permission**                                                                                                  |
| -	        | The file cannot be executed. | The directory cannot be accessed with cd.                                                  |
| x         | The file can be executed.    | The directory can be accessed with cd.                                                     |
| s           <td colspan=2>The setuid bit, allows a user to execute that file or program with the permission of the owner of that file |
| S           <td colspan=2>Same as s, but x is not set; rare on regular files, and useless on folders                                  |
| t           <td colspan=2>The sticky bit, allow the files in that directory to only be removed by the owner.                          |
| T           <td colspan=2>Same as t, but x is not set; rare on regular files, and useless on folders.                                 |

_Source: https://www.liquidweb.com/kb/how-do-i-set-up-setuid-setgid-and-sticky-bits-on-linux/_
