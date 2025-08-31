namespace Centauri;

public abstract class CentauriObject : IDisposable
{
    private IntPtr handle_;

    public void Dispose()
    {
        DisposeCore(true);
    }

    protected virtual void DisposeCore(bool disposing)
    {
        if (disposing)
        {

        }
    }
}

