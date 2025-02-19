import random
import string
import json
import sys

class Database:
    def __init__(self):
        self.db = {}
        self.seed = {
            "bins": {}
        }

        try:
            with open("database.json", "r") as f:
                self.db = json.load(f)
        except (FileNotFoundError, json.JSONDecodeError):
            with open("database.json", "w") as f:
                json.dump(self.seed, f)
            self.db = self.seed
        except Exception as e:
            print(f"[-] Cant start database class. {e}")
            sys.exit(1)

    def random_id(self, length=8):
        chars = string.ascii_letters + string.digits
        return "".join(random.choices(chars, k=length))

    def get_bin(self, id):
        if id in self.db["bins"]:
            return self.db["bins"][id]
        else:
            return None

    def create_bin(self, bin):
        id = self.random_id()
        self.db["bins"][id] = bin
        self.save()
        return id

    def save(self):
        try:
            with open("database.json", "w") as f:
                json.dump(self.db, f)
        except Exception as e:
            print(f"[-] Cant save database. {e}")
            sys.exit(1)
