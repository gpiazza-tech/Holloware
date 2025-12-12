from hw import Vec3
from hw import Math

class Coin(ScriptableObject):
    def __init__(self):
        self.hover_speed = 3.0
        self.hover_amplitude = 0.5
        self.rotation_speed = 2.0
        
        self._time = 0.0
        self._start_y = 0.0
    def on_start(self):
        self._start_y = self.position.y
    def on_update(self, ts):
        self._time += ts.get_seconds
        self.position.y = self._start_y + Math.sin(self._time * self.hover_speed) * self.hover_amplitude

        self.rotation.y += ts.get_seconds * self.rotation_speed

