#pragma once

class Tool {
public:
    int ID;

public:
    Tool() = default;
    virtual ~Tool() { }

    virtual void OnButtonPress() { }
    virtual void OnButtonDown() { }
    virtual void OnButtonRelease() { }
    virtual void Render() { }
};