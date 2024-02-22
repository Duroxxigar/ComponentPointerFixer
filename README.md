# Component Pointer Fixer

This plugin attempts to fix the pointer that gets borked when doing live coding or changing the CDSO.
The problem is that sometimes, the pointer doesn't point to the "new" component. So this causes the details
panel to be blank. This plugin attempts to find the new object and update the pointer to point to it.
I can't guarantee that this will 100% work every single time, but the alternative is to recreate the BP so
what's the worst that could happen? This doesn't fix changing the component attachment hierarchy.

# How to use

Just right click on the borked component and select the option in the submenu to fix the pointer. Can also
select multiple components at once.

# Special Thanks

- rweber69 - for showing how to add the menu option on the right click in the BP editor
- Authaer - for helping show how to find the new component