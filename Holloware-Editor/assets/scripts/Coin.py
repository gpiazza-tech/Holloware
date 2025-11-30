from hw import Vec3
from hw import Math

class Coin(ScriptableObject):
    # public
    hover_speed = 3
    hover_amplitude = 0.5

    rotation_speed = 2

    # private
    time = 0
    start_y = 0

    def on_start(self):
        self.start_y = self.position.y
    def on_update(self, ts):
        self.time += ts.get_seconds
        self.position.y = self.start_y + Math.sin(self.time * self.hover_speed) * self.hover_amplitude

        self.rotation.y += ts.get_seconds * self.rotation_speed

