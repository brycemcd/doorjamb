# IoT Doorjamb

> React to changes when my daughter wakes up to go to the bathroom at night

This is three projects in one:

1. Doorjamb Publisher
2. Parent Notifier
3. Bathroom Light

The doorjamb publisher is responsible for knowing if the door is open or closed
and if the light in the room indicates that my daughter should be in bed asleep
(nap time or sleep time)

If both the light and door conditions suggest that she should be asleep but the
door opens, the parent notifier and bathroom light should respond to those events.

The parent notifier blinks to alert us she's up and the bathroom LED lights turn
on to give her a destination to the toilet and maintain her night vision.

Communication happens with the publisher continuoulsy publishing door and light
status to an MQTT message broker. The subscriber devices pull messages off this
broker and respond to the published events in their own way given their responsibilities.
