import hw

class Player(ScriptableObject):
	def on_start(self):
		hw.log("Player Started!")
	def on_update(self):
		hw.log("Player Updated!")